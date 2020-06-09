#pragma once
#include "MathFwd.hpp"
#include "Path.hpp"

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

		IEngine(const IEngine&) = delete;
		IEngine(IEngine&&) = delete;
		IEngine& operator=(const IEngine&) = delete;
		IEngine& operator=(IEngine&&) = delete;
		
	protected:
		constexpr IEngine() noexcept = default;
		~IEngine() = default;
	};
}
