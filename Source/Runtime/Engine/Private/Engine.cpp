#include "Engine.hpp"
#include <filesystem>
#include <fstream>
#include <SDL2/SDL.h>
#include "Log.hpp"
#include "Json.hpp"
#include "Platform.hpp"

namespace oeng
{
	namespace fs = std::filesystem;

	static void LoadConfig(HashMap<Name, Json>& configs, const fs::path& file)
	{
		if (!is_regular_file(file) || file.extension() != ".json") return;
		
		auto name = file.stem().string();
		auto parsed = ReadFileAsJson(file);
		
		auto [it, inserted] = configs.try_emplace(std::move(name), std::move(parsed));
		if (inserted) return;
		
		for (auto& [key, value] : parsed.items())  // NOLINT(bugprone-use-after-move)
		{
			it->second[key] = std::move(value);
		}
	}
	
	static void LoadConfigs(HashMap<Name, Json>& configs, const fs::path& directory)
	{
		if (!exists(directory)) return;
		
		for (const auto& entry : fs::directory_iterator{directory})
		{
			try
			{
				LoadConfig(configs, entry);
			}
			catch (const std::exception& e)
			{
				log::Error("Failed to load config '{}': {}", entry.path().string(), e.what());
			}
		}
	}
	
	static HashMap<Name, Json> LoadConfigs()
	{
		HashMap<Name, Json> configs;

		LoadConfigs(configs, "../Engine/Config");
		LoadConfigs(configs, "../Config");
		
		return configs;
	}
	
	Engine::Engine(std::string_view game_name, const Function<void(Engine&)>& load_game)
		:game_name_{game_name},
		configs_{LoadConfigs()},
		renderer_{*this},
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

	Vec2u16 Engine::GetWindowSize() const noexcept
	{
		return renderer_.GetWindowSize();
	}

	bool Engine::SaveConfig(Name name) noexcept
	{
		try
		{
			fs::create_directory("../Config");
			std::ofstream file{fmt::format("../Config/{}.json", *name)};
			file.exceptions(std::ios_base::failbit);
			file << configs_[name].dump(4);
			return true;
		}
		catch (const std::exception& e)
		{
			log::Error("Failed to save config '{}': {}", *name, e.what());
			return false;
		}
	}

	Json& Engine::Config(Name name)
	{
		return configs_[name];
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

		input_system_.PostAddAllEvents();
	}

	extern OE_IMPORT bool engine_exist;

	SdlRaii::SdlRaii()
	{
		const auto& cpu = plf::CpuInfo::Get();
		log::Info("CPU: {}", cpu.GetBrand());
		if (!cpu.AVX2()) throw std::runtime_error{"Unsupported CPU (AVX2)"};
		
		if (engine_exist) throw std::runtime_error{"Only 1 engine instance can exists"};
		engine_exist = true;

		if (!IsGameThread()) throw std::runtime_error{"The engine instance must be created on the main thread"};
		
		log::Info("Initializing engine...");
		omem::SetOnPoolDest([](auto&&...){});
		
		const auto sdl_result = SDL_Init(SDL_INIT_EVERYTHING);
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

		engine_exist = false;
	}

	void Main(std::string_view game_name, const Function<void(Engine&)>& load_game)
	{
		if (plf::IsDebuggerPresent())
		{
			Engine{game_name, load_game}.RunLoop();
		}
		else try
		{
			Engine{game_name, load_game}.RunLoop();
		}
		catch (const std::exception& e)
		{
			log::Critical(e.what());
		}
	}
}
