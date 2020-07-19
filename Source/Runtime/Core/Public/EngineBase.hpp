#pragma once
#include <thread>
#include "Name.hpp"
#include "Config.hpp"

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
		
		[[nodiscard]] std::u8string_view GetGameName() const noexcept
		{
			return game_name_;
		}
		
		/**
		 * \brief Get memory pool manager.
		 * \return Reference to memory pool manager.
		 * \note It should only be called from a game thread.
		 */
		[[nodiscard]] omem::MemoryPoolManager& GetMemPool() noexcept
		{
			assert(IsGameThread());
			return mem_pool_;
		}
		
		/**
		 * \brief Check if the current thread is a game thread.
		 * \return True if called from the game thread.
		 */
		[[nodiscard]] bool IsGameThread() const noexcept
		{
			return thread_id_ == std::this_thread::get_id();
		}

	private:
		friend Name;
		
		std::thread::id thread_id_;
		std::u8string_view game_name_;
		omem::MemoryPoolManager mem_pool_;
		NameSet names_;
		ConfigManager config_;
	};

	extern CORE_API EngineBase* const kEngineBase;
}
