#pragma once
#include "JsonFwd.hpp"

namespace oeng::engine
{
	class World;
	class InputSystem;
}

namespace oeng::renderer
{
	class Renderer;
}

namespace oeng
{
	using namespace engine;
	using namespace renderer;
}

namespace oeng::core
{
	class OEAPI IEngine
	{
	public:
		INTERFACE_BODY(IEngine);
		
		[[nodiscard]] virtual World& GetWorld() noexcept = 0;
		[[nodiscard]] virtual InputSystem& GetInputSystem() noexcept = 0;
		[[nodiscard]] virtual Renderer& GetRenderer() noexcept = 0;

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
	};
}
