#pragma once
#include <memory>
#include "Math.hpp"

struct SDL_Window;

namespace oeng
{
	class SpriteComponent;

	using WindowPtr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;
	using GlContextPtr = std::unique_ptr<void, void(*)(void*)>;

	class Renderer
	{
	public:
		explicit Renderer(Vec2u16 scr);
		~Renderer();
		
		void RegisterSprite(const SpriteComponent& sprite);
		void UnregisterSprite(const SpriteComponent& sprite);

		void DrawScene() const;

		[[nodiscard]] Vec2u16 GetScreenSize() const noexcept;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;

	private:
		WindowPtr window_;
		GlContextPtr gl_context_;
		std::unique_ptr<class Shader> sprite_shader_;
		std::unique_ptr<class VertexArray> sprite_verts_;
		std::vector<std::reference_wrapper<const SpriteComponent>> sprites_;
	};
}
