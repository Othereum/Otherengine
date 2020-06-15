#pragma once
#include "MathFwd.hpp"
#include "Path.hpp"
#include "JsonFwd.hpp"

namespace oeng
{
	class Texture;
	class Mesh;
	class World;
	class InputSystem;
	class Renderer;
	
	class IEngine
	{
	public:
		[[nodiscard]] virtual World& GetWorld() noexcept = 0;
		[[nodiscard]] virtual InputSystem& GetInputSystem() noexcept = 0;
		[[nodiscard]] virtual Renderer& GetRenderer() noexcept = 0;
		[[nodiscard]] virtual Vec2u16 GetScreenSize() const noexcept = 0;
		[[nodiscard]] virtual std::string_view GetGameName() const noexcept = 0;
		[[nodiscard]] virtual const Json& GetConfig(Name name) const noexcept = 0;
		
		constexpr IEngine() noexcept = default;
		virtual ~IEngine() = default;
		
		IEngine(const IEngine&) = delete;
		IEngine(IEngine&&) = delete;
		IEngine& operator=(const IEngine&) = delete;
		IEngine& operator=(IEngine&&) = delete;
	};
}
