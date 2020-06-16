#pragma once
#include "MathFwd.hpp"
#include "Path.hpp"
#include "JsonFwd.hpp"
#include "Interfaces/Base.hpp"

namespace oeng
{
	class Texture;
	class Mesh;
	class World;
	class InputSystem;
	class Renderer;
	
	class IEngine
	{
		OE_INTERFACE_BODY(IEngine);
		
		[[nodiscard]] virtual World& GetWorld() noexcept = 0;
		[[nodiscard]] virtual InputSystem& GetInputSystem() noexcept = 0;
		[[nodiscard]] virtual Renderer& GetRenderer() noexcept = 0;
		[[nodiscard]] virtual Vec2u16 GetWindowSize() const noexcept = 0;
		[[nodiscard]] virtual std::string_view GetGameName() const noexcept = 0;

		/**
		 * \brief Find config or create if not found.
		 * \param name Config name
		 * \return Reference to non-const json config object
		 */
		[[nodiscard]] virtual Json& Config(Name name) = 0;
		
		/**
		 * \brief Save config as file. Existing files will be overwritten.
		 * \param name Config name
		 * \return true if successful
		 */
		virtual bool SaveConfig(Name name) noexcept = 0;
	};
}
