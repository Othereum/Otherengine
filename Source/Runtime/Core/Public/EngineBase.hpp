#pragma once
#include <thread>
#include "Config.hpp"
#include "Log.hpp"
#include "Path.hpp"
#include "Platform.hpp"
#include "Stat.hpp"

namespace oeng
{
	namespace engine
	{
		class Engine;
	}
	
	using namespace engine;
}

namespace oeng::core
{
	class EngineBase;
	
	class CORE_API RegisterEngineBase
	{
		friend EngineBase;
		DELETE_CPMV(RegisterEngineBase);
		explicit RegisterEngineBase(EngineBase* engine);
		~RegisterEngineBase();
	};
	
	class CORE_API CoreSystem
	{
	public:
		DELETE_CPMV(CoreSystem);
		
		/**
		 * Check if the current thread is a game thread.
		 * @return True if called from the game thread.
		 */
		[[nodiscard]] bool IsGameThread() const noexcept
		{
			return thread_id_ == std::this_thread::get_id();
		}

		/**
		 * Get memory pool manager.
		 * @return Reference to memory pool manager.
		 * @note It should only be called from a game thread.
		 */
		[[nodiscard]] omem::MemoryPoolManager& GetMemPool()
		{
			return mem_pools_[std::this_thread::get_id()];
		}
		
		[[nodiscard]] log::Logger& GetLogger() noexcept { return logger_; }
		[[nodiscard]] const Dll& GetGameDll() const noexcept { return game_dll_; }
		[[nodiscard]] std::u8string_view GetGameName() const noexcept { return game_name_; }
		
	private:
		friend EngineBase;
		explicit CoreSystem(std::u8string game_module_path);
		~CoreSystem();
		
		std::thread::id thread_id_;
		Dll game_dll_;
		
		std::u8string_view game_name_;
		log::Logger logger_;
		
		std::unordered_map<std::thread::id, omem::MemoryPoolManager> mem_pools_;
	};
	
	class CORE_API EngineBase : RegisterEngineBase, public CoreSystem
	{
	public:
		DELETE_CPMV(EngineBase);

		[[nodiscard]] static EngineBase& Get() noexcept;
		[[nodiscard]] static bool Exists() noexcept;
		
		[[nodiscard]] Config& GetConfig() noexcept { return config_; }
		[[nodiscard]] uint64_t GetTickCount() const noexcept { return ticks_; }
		
	protected:
		uint64_t ticks_;

	private:
		friend Name;
		friend Path;
		friend Engine;
		friend CounterManager;
		friend ScopeCounter;
		friend ScopeStackCounter;

		explicit EngineBase(std::u8string game_module_path);
		~EngineBase() = default;

		NameSet names_;
		PathSet paths_;
		Config config_;
		CounterManager counters_;
	};
}
