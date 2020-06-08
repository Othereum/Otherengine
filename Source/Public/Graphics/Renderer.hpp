#pragma once
#include "Graphics/Shader.hpp"
#include "Graphics/VertexArray.hpp"
#include "Templates/DyArr.hpp"

struct SDL_Window;

namespace oeng
{
	class Engine;
	class SpriteComponent;
	class MeshComponent;

	using WindowPtr = UniquePtr<SDL_Window, void(*)(SDL_Window*)>;
	using GlContextPtr = UniquePtr<void, void(*)(void*)>;

	class Renderer
	{
	public:
		explicit Renderer(Engine& engine, Vec2u16 scr);
		~Renderer();
		
		void RegisterSprite(const SpriteComponent& sprite);
		void UnregisterSprite(const SpriteComponent& sprite);

		void RegisterMesh(const MeshComponent& mesh);
		void UnregisterMesh(const MeshComponent& mesh);

		void DrawScene();

		[[nodiscard]] Vec2u16 GetScreenSize() const noexcept { return scr_sz_; }
		[[nodiscard]] Engine& GetEngine() const noexcept { return engine_; }

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;

	private:
		Engine& engine_;

		Vec2u16 scr_sz_;
		WindowPtr window_;
		GlContextPtr gl_context_;
		
		Shader basic_mesh_shader_;
		Shader sprite_shader_;
		VertexArray sprite_verts_;
		
		DyArr<std::reference_wrapper<const SpriteComponent>> sprites_;
		DyArr<std::reference_wrapper<const MeshComponent>> meshes_;
	};
}
