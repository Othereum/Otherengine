#pragma once
#include "Math.hpp"
#include "Templates/DyArr.hpp"

struct SDL_Window;

namespace oeng
{
	class Engine;
	class SpriteComponent;

	using WindowPtr = UniquePtr<SDL_Window, void(*)(SDL_Window*)>;
	using GlContextPtr = UniquePtr<void, void(*)(void*)>;

	class Renderer
	{
	public:
		explicit Renderer(Engine& engine, Vec2u16 scr);
		~Renderer();
		
		void RegisterSprite(const SpriteComponent& sprite);
		void UnregisterSprite(const SpriteComponent& sprite);

		void DrawScene() const;

		[[nodiscard]] Vec2u16 GetScreenSize() const noexcept;
		[[nodiscard]] Engine& GetEngine() const noexcept { return engine_; }

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;

	private:
		Engine& engine_;
		WindowPtr window_;
		GlContextPtr gl_context_;
		UniquePtr<class Shader> sprite_shader_;
		UniquePtr<class VertexArray> sprite_verts_;
		DyArr<std::reference_wrapper<const SpriteComponent>> sprites_;
	};
}
