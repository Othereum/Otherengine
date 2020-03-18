#include "Engine.h"

#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>

#include "Renderer.h"
#include "Actor/Actor.h"
#include "Component/InputComponent.h"
#include "Component/CircleComponent.h"

#include "Actor/Asteroid.h"
#include "Actor/Ship.h"
#include "MathUtil.h"

namespace Game
{
	CEngine::CEngine() :
		renderer_{std::make_unique<CRenderer>()},
		time_{std::chrono::steady_clock::now()}
	{
		for (auto i = 0; i < 20; ++i)
		{
			auto& ast = SpawnActor<AAsteroid>();
			ast.SetPos(Math::RandVec({0, 0}, TFVector2{kScrSz}));
			ast.SetRot(Math::RandAng());
		}

		auto& sh = SpawnActor<ship>();
		sh.SetPos(TFVector2{kScrSz / 2});
	}

	CEngine::~CEngine() = default;

	void CEngine::RunLoop()
	{
		while (bIsRunning_)
		{
			ProcessInput();
			UpdateGame();
			GenerateOutput();
		}
	}

	void CEngine::Shutdown()
	{
		bIsRunning_ = false;
	}

	void CEngine::RegisterInputComponent(const CInputComponent& comp)
	{
		auto cmp = [](const CInputComponent& a, const CInputComponent& b)
		{
			return a.GetReceiveOrder() <= b.GetReceiveOrder();
		};
		const auto pos = std::lower_bound(inputComps_.begin(), inputComps_.end(), comp, cmp);
		inputComps_.emplace(pos, comp);
	}

	void CEngine::UnregisterInputComponent(const CInputComponent& comp)
	{
		auto pr = [&](const CInputComponent& v) { return &v == &comp; };
		const auto found = std::find_if(inputComps_.crbegin(), inputComps_.crend(), pr);
		if (found != inputComps_.crend()) inputComps_.erase(found.base() - 1);
	}

	void CEngine::RegisterCollision(CCircleComponent& comp)
	{
		collisions_.emplace_back(comp);
	}

	void CEngine::UnregisterCollision(CCircleComponent& comp)
	{
		auto pr = [&](const CCircleComponent& v) { return &v == &comp; };
		const auto found = std::find_if(collisions_.crbegin(), collisions_.crend(), pr);
		if (found != collisions_.crend()) collisions_.erase(found.base() - 1);
	}

	std::shared_ptr<SDL_Texture> CEngine::GetTexture(const char* filename)
	{
		const auto found = textures_.find(filename);
		if (found != textures_.end()) return found->second.lock();

		const auto loaded = LoadTexture(filename);
		textures_.emplace(filename, loaded);

		return loaded;
	}

	void CEngine::ProcessInput()
	{
		std::vector<SDL_Keycode> keyEvents[2];
		
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				Shutdown();
				return;

			case SDL_KEYDOWN: case SDL_KEYUP:
				if (!event.key.repeat) keyEvents[event.key.state].push_back(event.key.keysym.sym);
				break;
				
			default: ;
			}
		}

		const auto keyboard = SDL_GetKeyboardState(nullptr);
		for (auto comp : inputComps_)
		{
			comp.get().ProcessInput(keyEvents, keyboard);
		}
	}

	void CEngine::UpdateGame()
	{
		const auto deltaSeconds = UpdateTime();
		for (const auto& actor : actors_)
			actor->Update(deltaSeconds);

		
		for (size_t i = 0; i < collisions_.size(); ++i)
			for (auto j = i+1; j < collisions_.size(); ++j)
				collisions_[i].get().TestOverlap(collisions_[j]);

		
		for (auto&& pending : pendingActors_)
		{
			pending->BeginPlay();
			actors_.push_back(std::move(pending));
		}
		pendingActors_.clear();

		
		for (auto it = actors_.rbegin(); it != actors_.rend();)
		{
			const auto& actor = **it;
			if (actor.GetState() == AActor::EState::dead)
			{
				const auto next = actors_.erase(it.base() - 1);
				it = std::make_reverse_iterator(next);
			}
			else
			{
				++it;
			}
		}
	}

	void CEngine::GenerateOutput() const
	{
		renderer_->DrawScene();
	}

	float CEngine::UpdateTime()
	{
		using namespace std::chrono;
		const auto now = steady_clock::now();
		const auto deltaSeconds = duration<float>{now - time_}.count();
		time_ = now;
		return deltaSeconds;
	}

	void CEngine::RegisterActor(std::unique_ptr<AActor>&& actor)
	{
		pendingActors_.push_back(std::move(actor));
	}

	std::shared_ptr<SDL_Texture> CEngine::LoadTexture(const char* filename)
	{
		const std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)> surface{IMG_Load(filename), SDL_FreeSurface};
		if (!surface) throw std::runtime_error{SDL_GetError()};

		auto deleter = [this, filename](SDL_Texture* const texture)
		{
			textures_.erase(filename);
			SDL_DestroyTexture(texture);
		};
		std::shared_ptr<SDL_Texture> texture{SDL_CreateTextureFromSurface(&renderer_->GetSdlRenderer(), surface.get()), std::move(deleter)};
		if (!texture) throw std::runtime_error{SDL_GetError()};

		return texture;
	}

	CSdlRaii::CSdlRaii()
	{
		const auto sdlResult = SDL_Init(SDL_INIT_VIDEO);
		if (sdlResult != 0) throw std::runtime_error{SDL_GetError()};

		const auto flags = IMG_INIT_PNG;
		const auto imgResult = IMG_Init(flags);
		if (imgResult != flags) throw std::runtime_error{IMG_GetError()};
	}

	CSdlRaii::~CSdlRaii()
	{
		IMG_Quit();
		SDL_Quit();
	}
}
