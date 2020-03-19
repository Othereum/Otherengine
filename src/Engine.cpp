#include "Engine.h"

#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>

#include "Component/InputComponent.h"
#include "Component/CircleComponent.h"
#include "InputSystem.h"
#include "Renderer.h"
#include "World.h"

#include "Actor/Asteroid.h"
#include "Actor/Ship.h"
#include "MathUtil.h"

namespace Game
{
	CEngine::CEngine() :
		world_{std::make_unique<CWorld>(*this)},
		inputSystem_{std::make_unique<CInputSystem>()}
	{
		inputSystem_->AddAxis("MoveForward", {
			{'w', EInputType::keyboard, 1},
			{'s', EInputType::keyboard, -1},
		});
		
		inputSystem_->AddAxis("Turn", {
			{'a', EInputType::keyboard, -1},
			{'d', EInputType::keyboard, 1},
		});

		inputSystem_->AddAction("Shoot", {
			{' ', EInputType::keyboard}
		});
		
		for (auto i = 0; i < 20; ++i)
		{
			auto& ast = world_->SpawnActor<AAsteroid>();
			ast.SetPos(Math::RandVec({0, 0}, TFVector2{kScrSz}));
			ast.SetRot(Math::RandAng());
		}

		auto& sh = world_->SpawnActor<ship>();
		sh.SetPos(TFVector2{kScrSz / 2});
	}

	CEngine::~CEngine() = default;

	void CEngine::RunLoop()
	{
		while (bIsRunning_)
		{
			Tick();
		}
	}

	void CEngine::Shutdown()
	{
		bIsRunning_ = false;
	}

	std::shared_ptr<SDL_Texture> CEngine::GetTexture(FName file)
	{
		const auto found = textures_.find(file);
		if (found != textures_.end()) return found->second.lock();

		const auto loaded = LoadTexture(file);
		textures_.emplace(file, loaded);

		return loaded;
	}

	void CEngine::Tick()
	{
		ProcessEvent();
		world_->Tick();
	}

	void CEngine::ProcessEvent()
	{
		inputSystem_->ClearEvents();
		
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				Shutdown();
				return;
			}
			
			inputSystem_->AddEvent(event);
		}
	}

	std::shared_ptr<SDL_Texture> CEngine::LoadTexture(FName file)
	{
		const std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)> surface{
			IMG_Load(file.Str().c_str()),
			SDL_FreeSurface
		};
		if (!surface) throw std::runtime_error{SDL_GetError()};

		std::shared_ptr<SDL_Texture> texture{
			SDL_CreateTextureFromSurface(&world_->GetRenderer().GetSdlRenderer(), surface.get()),
			[this, file](SDL_Texture* texture)
			{
				textures_.erase(file);
				SDL_DestroyTexture(texture);
			}
		};
		if (!texture) throw std::runtime_error{SDL_GetError()};
		
		return texture;
	}

	CSdlRaii::CSdlRaii()
	{
		const auto sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
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
