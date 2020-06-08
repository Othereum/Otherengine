#pragma once
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "Templates/DyArr.hpp"

struct SDL_Window;

namespace oeng
{
	class ISprite;
	class IMesh;
	class IEngine;

	using WindowPtr = UniquePtr<SDL_Window, void(*)(SDL_Window*)>;
	using GlContextPtr = UniquePtr<void, void(*)(void*)>;

	class Renderer
	{
	public:
		explicit Renderer(IEngine& engine, Vec2u16 scr);
		~Renderer();
		
		void RegisterSprite(const ISprite& sprite);
		void UnregisterSprite(const ISprite& sprite);

		void RegisterMesh(const IMesh& mesh);
		void UnregisterMesh(const IMesh& mesh);

		void DrawScene();

		[[nodiscard]] Vec2u16 GetScreenSize() const noexcept { return scr_sz_; }
		[[nodiscard]] IEngine& GetEngine() const noexcept { return engine_; }

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;

	private:
		IEngine& engine_;

		Vec2u16 scr_sz_;
		WindowPtr window_;
		GlContextPtr gl_context_;
		
		Shader basic_mesh_shader_;
		Shader sprite_shader_;
		VertexArray sprite_verts_;
		
		DyArr<std::reference_wrapper<const ISprite>> sprites_;
		DyArr<std::reference_wrapper<const IMesh>> meshes_;
	};
}
