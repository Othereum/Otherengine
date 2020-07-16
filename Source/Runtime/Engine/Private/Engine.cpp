#include "Engine.hpp"
#include <filesystem>
#include <fstream>
#include <SDL2/SDL.h>
#include "Core.hpp"
#include "Debug.hpp"
#include "Log.hpp"
#include "Stat.hpp"

namespace oeng
{
	using namespace std::literals;
	namespace fs = std::filesystem;

	namespace core
	{
		extern OE_IMPORT bool engine_exist;
		extern OE_IMPORT ScopeCycleManager scope_cycle_manager;
		extern OE_IMPORT std::unordered_map<Name, ScopeStat> scope_stats;
	}

	static const fs::path& GetUserConfigDir()
	{
		static const auto dir = GetUserDataPath() / u8"Config";
		return dir;
	}

	static void LoadConfig(HashMap<Name, Json>& configs, const fs::path& file)
	{
		static const fs::path json = u8".json";
		if (!is_regular_file(file) || file.extension() != json) return;
		
		auto name = file.stem().u8string();
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
				log::Error(u8"Failed to load config '{}': {}", entry.path().u8string(), What(e));
			}
		}
	}
	
	static HashMap<Name, Json> LoadConfigs()
	{
		HashMap<Name, Json> configs;

		LoadConfigs(configs, u8"../Engine/Config");
		LoadConfigs(configs, u8"../Config");
		LoadConfigs(configs, GetUserConfigDir());
		
		return configs;
	}
	
	Engine::Engine(void load_game(Engine&))
		:configs_{LoadConfigs()},
		renderer_{*this},
		input_system_{*this},
		world_{*this}
	{
		log::Info(u8"Engine initialization successful.");
		log::Info(u8"Loading game module...");
		load_game(*this);
		log::Info(u8"Game module loaded.");
	}

	static void LogStat(Name name, const ScopeStat& stat, uint64_t ticks, int depth = 0)
	{
		const auto time = duration_cast<time::duration<Float, std::milli>>(stat.duration / ticks).count();
		const auto count = ToFloat(stat.count) / ToFloat(ticks);
		log::Debug(u8"[Stat]{:^{}} {} took {:.2f} ms, {:.1f} times"sv, u8""sv, depth, *name, time, count);
	}

	static void LogStats(const std::map<Name, ScopeCycleStat>& stats, uint64_t ticks, int depth = 0)
	{
		for (auto& [name, stat] : stats)
		{
			LogStat(name, stat, ticks, depth);
			LogStats(stat.children, ticks, depth + 1);
		}
	}
	
	static void LogStats(uint64_t ticks)
	{
		if (ticks == 0) return;
		
		LogStats(scope_cycle_manager.Stats(), ticks);

		for (auto& [name, stat] : scope_stats)
		{
			LogStat(name, stat, ticks);
		}
	}

	Engine::~Engine()
	{
		try { LogStats(ticks_); }
		catch (const std::exception& e)
		{
			log::Error(u8"Failed to log stats: {}", What(e));
			DEBUG_BREAK();
		}
	}

	void Engine::RunLoop()
	{
		world_.BeginTick();

		log::Info(u8"Engine loop started.");
		const auto start = Clock::now();

		while (is_running_)
		{
			Tick();
			++ticks_;
		}

		const auto elapsed = Clock::now() - start;
		const auto sec = duration_cast<time::seconds>(elapsed).count();
		if (sec != 0 && ticks_ != 0)
		{
			const auto ms = duration_cast<time::duration<Float, std::milli>>(elapsed).count();
			log::Info(u8"Average fps: {}, frame time: {:.2f} ms", ticks_ / sec, ms / ticks_);
		}
	}

	void Engine::Shutdown()
	{
		is_running_ = false;
		log::Info(u8"Engine shutdown requested.");
	}

	bool Engine::SaveConfig(Name name)
	{
		try
		{
			auto& dir = GetUserConfigDir();
			create_directories(dir);
			
			std::ofstream file{dir / Format(u8"{}.json", *name)};
			file.exceptions(std::ios_base::failbit | std::ios_base::badbit);
			
			file << configs_[name].dump(4);
			return true;
		}
		catch (const std::exception& e)
		{
			log::Error(u8"Failed to save config '{}': {}", *name, What(e));
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

	static void LogMemoryInfo()
	{
		auto& pools = omem::GetPools();
		
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
			log::Debug(u8"[Mem] {:>{}}-byte blocks, total: {:>{}}, peak: {:>{}}, fault: {:>{}}, leaked: {}",
				info.size, align.size, info.count, align.count, info.peak, align.peak, info.fault, align.fault, info.cur);
		}

		if (max.cur > 0) log::Warn(u8"[Mem] Memory leak detected!");
	}

	SdlRaii::SdlRaii()
	{
		assert(IsGameThread());
		
		assert(!engine_exist);
		engine_exist = true;

		log::Info(u8"Initializing engine...");
		
		const auto sdl_result = SDL_Init(SDL_INIT_EVERYTHING);
		if (sdl_result != 0) throw std::runtime_error{SDL_GetError()};
	}

	SdlRaii::~SdlRaii()
	{
		SDL_Quit();
		
		try { LogMemoryInfo(); }
		catch (const std::exception& e)
		{
			log::Error(u8"Failed to log memory info: {}", What(e));
			DEBUG_BREAK();
		}
		
		engine_exist = false;
	}
}
