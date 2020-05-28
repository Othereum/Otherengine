#include "Engine.hpp"

#include <stdexcept>
#include <SDL.h>

#include "GameModule.hpp"
#include "Components/InputComponent.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Mesh.hpp"
#include "InputSystem.hpp"
#include "World.hpp"

namespace oeng
{
	Engine::Engine():
		renderer_{std::make_unique<Renderer>(Vec2u16{1024, 768})},
		world_{std::make_unique<World>(*this)},
		input_system_{std::make_unique<InputSystem>()}
	{
		LoadGameModule(*this);
	}

	Engine::~Engine() = default;

	void Engine::RunLoop()
	{
		while (is_running_)
		{
			Tick();
		}
	}

	void Engine::Shutdown()
	{
		is_running_ = false;
	}

	std::shared_ptr<Texture> Engine::GetTexture(Path file)
	{
		const auto found = textures_.find(file);
		if (found != textures_.end()) return found->second.lock();

		std::shared_ptr<Texture> loaded{
			new Texture{file},
			[this, file](Texture* p) noexcept
			{
				textures_.erase(file);
				delete p;
			}
		};

		textures_.emplace(file, loaded);
		return loaded;
	}

	std::shared_ptr<Mesh> Engine::GetMesh(Path file)
	{
		const auto found = meshes_.find(file);
		if (found != meshes_.end()) return found->second.lock();

		std::shared_ptr<Mesh> loaded{
			new Mesh{file, *this},
			[this, file](Mesh* p) noexcept
			{
				meshes_.erase(file);
				delete p;
			}
		};

		meshes_.emplace(file, loaded);
		return loaded;
	}

	Vec2u16 Engine::GetScreenSize() const noexcept
	{
		return renderer_->GetScreenSize();
	}

	void Engine::Tick()
	{
		ProcessEvent();
		world_->Tick();
		renderer_->DrawScene();
	}

	void Engine::ProcessEvent()
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

	SdlRaii::SdlRaii()
	{
		const auto sdl_result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
		if (sdl_result != 0) throw std::runtime_error{SDL_GetError()};
	}

	SdlRaii::~SdlRaii()
	{
		SDL_Quit();
	}
}
