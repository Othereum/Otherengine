#include "Engine.hpp"
#include <SDL.h>
#include <spdlog/async.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Log.hpp"
#include "Format.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Mesh.hpp"

namespace oeng
{
	Engine::Engine(std::string_view game_name, const Function<void(Engine&)>& load_game)
		:SdlRaii{game_name},
		game_name_{game_name},
		renderer_{*this, {1024, 768}},
		world_{*this}
	{
		load_game(*this);
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

	SharedPtr<Texture> Engine::GetTexture(Path file)
	{
		const auto found = textures_.find(file);
		if (found != textures_.end()) return found->second.lock();

		SharedPtr<Texture> loaded{
			New<Texture>(file),
			[this, file](Texture* p) noexcept
			{
				textures_.erase(file);
				Delete(p);
			}
		};

		textures_.emplace(file, loaded);
		return loaded;
	}

	SharedPtr<Mesh> Engine::GetMesh(Path file)
	{
		const auto found = meshes_.find(file);
		if (found != meshes_.end()) return found->second.lock();

		SharedPtr<Mesh> loaded{
			New<Mesh>(file, *this),
			[this, file](Mesh* p) noexcept
			{
				meshes_.erase(file);
				Delete(p);
			}
		};

		meshes_.emplace(file, loaded);
		return loaded;
	}

	Vec2u16 Engine::GetScreenSize() const noexcept
	{
		return renderer_.GetScreenSize();
	}

	void Engine::Tick()
	{
		ProcessEvent();
		world_.Tick();
		renderer_.DrawScene();
	}

	void Engine::ProcessEvent()
	{
		input_system_.ClearEvents();
		
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				Shutdown();
				return;
			}
			
			input_system_.AddEvent(event);
		}
	}

	void InitLog(std::string_view game_name)
	{
		std::filesystem::create_directory("../Logs");
		auto daily_file = std::make_shared<log::sinks::daily_file_sink_mt>(format("../Logs/{}.log", game_name), 0, 0);
		auto stdout_color = std::make_shared<log::sinks::stdout_color_sink_mt>();
		log::sinks_init_list sinks{std::move(daily_file), std::move(stdout_color)};
		auto logger = std::make_shared<log::async_logger>("", sinks, log::thread_pool());
		set_default_logger(std::move(logger));
	}

	SdlRaii::SdlRaii(std::string_view game_name)
	{
		InitLog(game_name);
		
		const auto sdl_result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
		if (sdl_result != 0) throw std::runtime_error{SDL_GetError()};
	}

	SdlRaii::~SdlRaii()
	{
		SDL_Quit();
	}
}
