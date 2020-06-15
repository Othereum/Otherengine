#include "Engine.hpp"
#include <filesystem>
#include <SDL.h>
#include "Log.hpp"
#include "Json.hpp"

namespace oeng
{
	static auto LoadConfigs()
	{
		namespace fs = std::filesystem;
		HashMap<Name, Json> configs;

		auto load = [&](const fs::path& path)
		{
			if (!exists(path)) return;
			
			for (const auto& entry : fs::directory_iterator{path}) try
			{
				if (!entry.is_regular_file()) continue;
				
				auto name = entry.path().stem().string();
				auto parsed = ReadFileAsJson(entry.path());
				auto [it, inserted] = configs.try_emplace(std::move(name), std::move(parsed));
				
				if (inserted) continue;
				
				for (auto& [key, value] : parsed.items())  // NOLINT(bugprone-use-after-move)
				{
					it->second[key] = std::move(value);
				}
			}
			catch (const std::exception& e)
			{
				log::Error("Failed to load config '{}': {}", entry.path().string(), e.what());
			}
		};

		load("../Engine/Config");
		load("../Config");
		
		return configs;
	}
	
	Engine::Engine(std::string_view game_name, const Function<void(Engine&)>& load_game)
		:game_name_{game_name},
		configs_{LoadConfigs()},
		renderer_{*this, configs_.at("Engine").at("resolution").get<Vec2u16>()},
		world_{*this}
	{
		log::Info("Engine initialization successful.");
		log::Info("Loading game module...");
		load_game(*this);
		log::Info("Game module loaded.");
	}

	Engine::~Engine() = default;

	void Engine::RunLoop()
	{
		log::Info("Engine loop started.");
		
		const auto start = Clock::now();
		auto tick = 0ull;
		
		while (is_running_)
		{
			Tick();
			++tick;
		}

		const auto sec = duration_cast<std::chrono::seconds>(Clock::now() - start).count();
		if (sec > 0) log::Info("Average fps: {}", tick / sec);
	}

	void Engine::Shutdown()
	{
		is_running_ = false;
		log::Info("Engine shutdown requested.");
	}

	Vec2u16 Engine::GetScreenSize() const noexcept
	{
		return renderer_.GetScreenSize();
	}

	const Json& Engine::GetConfig(Name name) const noexcept
	{
		if (const auto found = configs_.find(name); found != configs_.end())
			return found->second;

		static const Json default_json;
		return default_json;
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

	extern OE_IMPORT bool engine_exist;

	SdlRaii::SdlRaii()
	{
		if (engine_exist) throw std::runtime_error{"Only 1 engine instance can exists"};
		engine_exist = true;

		if (!IsGameThread()) throw std::runtime_error{"The engine instance must be created on the main thread"};
		
		log::Info("Initializing engine...");
		omem::SetOnPoolDest([](auto&&...){});
		
		const auto sdl_result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
		if (sdl_result != 0) throw std::runtime_error{SDL_GetError()};
	}

	SdlRaii::~SdlRaii()
	{
		SDL_Quit();

		for (const auto& [size, pool] : omem::GetPools())
		{
			const auto& info = pool.GetInfo();
			log::Info("[Mem] Memory pool with {} {}-byte blocks", info.count, info.size);
			log::Info("[Mem]  Peak usage: {} blocks", info.peak);
			if (info.fault) log::Info("[Mem]  Block fault: {} times", info.fault);
			if (info.cur) log::Warn("[Mem]  Leaked: {} blocks", info.cur);
		}
	}
}
