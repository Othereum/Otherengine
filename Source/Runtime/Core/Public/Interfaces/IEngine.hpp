#pragma once
#include "JsonFwd.hpp"

namespace oeng
{
	class Texture;
	class Mesh;
	class World;
	class InputSystem;
	class Renderer;
	
	class OEAPI IEngine
	{
	public:
		OE_INTERFACE_BODY(IEngine);
		
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
