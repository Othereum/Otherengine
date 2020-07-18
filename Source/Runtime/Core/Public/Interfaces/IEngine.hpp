#pragma once
#include "Core.hpp"
#include "JsonFwd.hpp"

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
	struct Name;

	class CORE_API IEngine
	{
	public:
		INTERFACE_BODY(IEngine);
		
		[[nodiscard]] virtual World& GetWorld() noexcept = 0;
		[[nodiscard]] virtual InputSystem& GetInputSystem() noexcept = 0;
		[[nodiscard]] virtual Renderer& GetRenderer() noexcept = 0;
		[[nodiscard]] virtual const char8_t* GetGameName() noexcept = 0;
		
		/**
		 * \brief Get memory pool manager.
		 * \return Reference to memory pool manager.
		 * \note It should only be called from a game thread.
		 */
		[[nodiscard]] omem::MemoryPoolManager& GetMemPool() noexcept
		{
			assert(IsGameThread());
			return GetMemPoolImpl();
		}
		
		/**
		 * \brief Check if the current thread is a game thread.
		 * \return True if called from the game thread.
		 */
		[[nodiscard]] virtual bool IsGameThread() noexcept = 0;

		/**
		 * \brief Find config or create if not found.
		 * \param name Config name
		 * \return Reference to non-const json config object
		 */
		[[nodiscard]] virtual Json& Config(Name name) = 0;
		
		/**
		 * \brief Save config as file into the user data directory.
		 * Existing files will be overwritten.
		 * \param name Config name
		 * \return true if successful
		 */
		virtual bool SaveConfig(Name name) = 0;

	protected:
		[[nodiscard]] virtual omem::MemoryPoolManager& GetMemPoolImpl() noexcept = 0; 
	};

	extern CORE_API IEngine* const kIEngine;
}
