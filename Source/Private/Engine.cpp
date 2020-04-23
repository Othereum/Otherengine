#include "Engine.h"

#include <stdexcept>
#include <SDL.h>

#include "Components/InputComponent.h"
#include "Components/CircleComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/Texture.hpp"
#include "InputSystem.h"
#include "World.h"

namespace oeng
{
	extern void LoadGameModule(CEngine&);
	
	CEngine::CEngine() :
		world_{std::make_unique<CWorld>(*this)},
		input_system_{std::make_unique<CInputSystem>()}
	{
		LoadGameModule(*this);
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

	std::shared_ptr<graphics::Texture> CEngine::GetTexture(Name file)
	{
		const auto found = textures_.find(file);
		if (found != textures_.end()) return found->second.lock();

		std::shared_ptr<graphics::Texture> loaded{
			new graphics::Texture{file.CStr()},
			[this, file](graphics::Texture* p) noexcept
			{
				textures_.erase(file);
				delete p;
			}
		};

		textures_.emplace(file, loaded);
		return loaded;
	}

	const Vector<uint16_t, 2>& CEngine::GetScreenSize() const noexcept
	{
		return graphics::kScrSz;
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

	CSdlRaii::CSdlRaii()
	{
		const auto sdl_result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
		if (sdl_result != 0) throw std::runtime_error{SDL_GetError()};
	}

	CSdlRaii::~CSdlRaii()
	{
		SDL_Quit();
	}
}
