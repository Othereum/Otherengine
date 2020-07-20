#pragma once
#include <thread>
#include "Config.hpp"
#include "Log.hpp"
#include "Path.hpp"
#include "Stat.hpp"

namespace oeng
{
	namespace engine
	{
		class Engine;
		class World;
		class InputSystem;
	}

	namespace renderer
	{
		class Renderer;
	}

	using namespace engine;
	using namespace renderer;
}

namespace oeng::core
{
	class CORE_API EngineBase
	{
	public:
		DELETE_CPMV(EngineBase);
		
		explicit EngineBase(std::u8string_view game_name);
		virtual ~EngineBase() = default;
		
		[[nodiscard]] virtual World& GetWorld() noexcept = 0;
		[[nodiscard]] virtual InputSystem& GetInputSystem() noexcept = 0;
		[[nodiscard]] virtual Renderer& GetRenderer() noexcept = 0;
		
		/**
		 * Get memory pool manager.
		 * @return Reference to memory pool manager.
		 * @note It should only be called from a game thread.
		 */
		[[nodiscard]] omem::MemoryPoolManager& GetMemPool() noexcept
		{
			assert(IsGameThread());
			return mem_pool_;
		}
		
		/**
		 * Check if the current thread is a game thread.
		 * @return True if called from the game thread.
		 */
		[[nodiscard]] bool IsGameThread() const noexcept
		{
			return thread_id_ == std::this_thread::get_id();
		}

		[[nodiscard]] log::Logger& GetLogger() noexcept { return logger_; }
		[[nodiscard]] std::u8string_view GetGameName() const noexcept { return game_name_; }

	private:
		friend Name;
		friend Path;
		friend ScopeCounter;
		friend ScopeStackCounter;

		std::thread::id thread_id_;
		std::u8string_view game_name_;
		omem::MemoryPoolManager mem_pool_;
		NameSet names_;
		PathSet paths_;
		
		log::Logger logger_;
		ConfigManager config_;
		CounterManager counters_;
	};

	extern CORE_API EngineBase* const kEngineBase;
}
