#include "Engine.h"

#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>

#include "Component/InputComponent.h"
#include "Component/CircleComponent.h"
#include "InputSystem.h"
#include "Graphics/Renderer.h"
#include "World.h"

#include "Actor/Asteroid.h"
#include "Actor/Ship.h"
#include "MathUtil.h"

namespace game
{
	CEngine::CEngine() :
		world_{std::make_unique<CWorld>(*this)},
		input_system_{std::make_unique<CInputSystem>()}
	{
		input_system_->AddAxis("MoveForward", {
			{'w', EInputType::keyboard, 1},
			{'s', EInputType::keyboard, -1},
		});
		
		input_system_->AddAxis("Turn", {
			{'a', EInputType::keyboard, -1},
			{'d', EInputType::keyboard, 1},
		});

		input_system_->AddAction("Shoot", {
			{' ', EInputType::keyboard}
		});
		
		for (auto i = 0; i < 20; ++i)
		{
			auto& ast = world_->SpawnActor<AAsteroid>();
			ast.SetPos(math::RandVec({0, 0}, FVector2{graphics::kScrSz}));
			ast.SetRot(math::RandAng());
		}

		auto& sh = world_->SpawnActor<ship>();
		sh.SetPos(FVector2{graphics::kScrSz / 2});
	}

	CEngine::~CEngine() = default;

	void CEngine::RunLoop()
	{
		while (is_running_)
		{
			Tick();
		}
	}

	void CEngine::Shutdown()
	{
		is_running_ = false;
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
		input_system_->ClearEvents();
		
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				Shutdown();
				return;
			}
			
			input_system_->AddEvent(event);
		}
	}

	std::shared_ptr<SDL_Texture> CEngine::LoadTexture(FName file)
	{
		return nullptr;
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
