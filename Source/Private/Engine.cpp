#include "Engine.h"

#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>

#include "Components/InputComponent.h"
#include "Components/CircleComponent.h"
#include "InputSystem.h"
#include "Graphics/Renderer.h"
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

	std::shared_ptr<SDL_Texture> CEngine::GetTexture(FName file)
	{
		const auto found = textures_.find(file);
		if (found != textures_.end()) return found->second.lock();

		auto loaded = LoadTexture(file);
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

	std::shared_ptr<SDL_Texture> CEngine::LoadTexture(FName file)
	{
		auto* const surface = IMG_Load(file.Str().c_str());
		if (!surface) throw std::runtime_error{SDL_GetError()};
		const std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)> surface_deleter{surface, SDL_FreeSurface};

		auto* const texture = SDL_CreateTextureFromSurface(&world_->GetRenderer().GetSdlRenderer(), surface);
		if (!texture) throw std::runtime_error{SDL_GetError()};

		return {texture, [this, file](SDL_Texture* texture)
			{
				textures_.erase(file);
				SDL_DestroyTexture(texture);
			}
		};
	}

	CSdlRaii::CSdlRaii()
	{
		const auto sdl_result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
		if (sdl_result != 0) throw std::runtime_error{SDL_GetError()};

		const auto flags = IMG_INIT_PNG;
		const auto img_result = IMG_Init(flags);
		if (img_result != flags) throw std::runtime_error{IMG_GetError()};
	}

	CSdlRaii::~CSdlRaii()
	{
		IMG_Quit();
		SDL_Quit();
	}
}
