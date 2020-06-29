#include "Engine.hpp"
#include <filesystem>
#include <fstream>
#include <SDL2/SDL.h>
#include "Log.hpp"
#include "Json.hpp"
#include "Core.hpp"
#include "Debug.hpp"

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

	static void LogMemoryInfo(const omem::PoolMap& pools)
	{
		omem::PoolInfo max;
		std::vector<omem::PoolInfo> infos;
		infos.reserve(pools.size());
		
		for (const auto& [size, pool] : pools)
		{
			auto compare = [](const omem::PoolInfo& a, const omem::PoolInfo& b) noexcept
			{
				return a.size < b.size;
			};
			const auto& info = pool.GetInfo();
			const auto pos = std::upper_bound(infos.begin(), infos.end(), info, compare);
			infos.insert(pos, info);

			max.size = Max(max.size, info.size);
			max.count = Max(max.count, info.count);
			max.cur = Max(max.cur, info.cur);
			max.peak = Max(max.peak, info.peak);
			max.fault = Max(max.fault, info.fault);
		}

		omem::PoolInfo align;
		align.size = Log(max.size, 10) + 1;
		align.count = Log(max.count, 10) + 1;
		align.peak = Log(max.peak, 10) + 1;
		align.fault = Log(max.fault, 10) + 1;
		
		for (const auto& info : infos)
		{
			log::Info("[Mem] {:>{}}-byte blocks, total: {:>{}}, peak: {:>{}}, fault: {:>{}}, leaked: {}",
				info.size, align.size, info.count, align.count, info.peak, align.peak, info.fault, align.fault, info.cur);
		}

		if (max.cur > 0) log::Warn("[Mem] Memory leak detected!");
	}

	SdlRaii::SdlRaii()
	{
		assert(IsGameThread());
		
		assert(!engine_exist);
		engine_exist = true;

		log::Info("Initializing engine...");
		
		const auto sdl_result = SDL_Init(SDL_INIT_EVERYTHING);
		if (sdl_result != 0) throw std::runtime_error{SDL_GetError()};
	}

	SdlRaii::~SdlRaii()
	{
		SDL_Quit();
		
		try { LogMemoryInfo(omem::GetPools()); }
		catch (...) { DebugBreak(); }
		
		engine_exist = false;
	}

	void Main(std::string_view game_name, const Function<void(Engine&)>& load_game)
	{
		if (!SDL_HasAVX2()) throw std::runtime_error{"Unsupported CPU (AVX2)"};
		
		if (IsDebugging())
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
