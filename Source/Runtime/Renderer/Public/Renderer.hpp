#pragma once
#include "Path.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "Templates/DyArr.hpp"
#include "Interfaces/Camera.hpp"

struct SDL_Window;

namespace oeng
{
	class IMesh;
	class ISprite;
	class IEngine;
	class ICamera;
	class Texture;
	class Mesh;

	using WindowPtr = UniquePtr<SDL_Window, void(*)(SDL_Window*)>;
	using GlContextPtr = UniquePtr<void, void(*)(void*)>;

	class DefaultCamera final : public ICamera
	{
	public:
		[[nodiscard]] const Mat4& GetViewProj() const noexcept override;
		void OnScreenSizeChanged(Vec2u16 scr) override;

	private:
		Mat4 view_proj_;
	};
	
	class OEAPI Renderer
	{
	public:
		explicit Renderer(IEngine& engine, Vec2u16 scr);
		~Renderer();
		
		void RegisterSprite(const ISprite& sprite);
		void UnregisterSprite(const ISprite& sprite);

		void RegisterMesh(const IMesh& mesh);
		void UnregisterMesh(const IMesh& mesh);

		void RegisterCamera(ICamera& camera) noexcept
		{
			camera_ = &camera;
			camera.OnScreenSizeChanged(GetScreenSize());
		}
		
		void UnregisterCamera(ICamera& camera) noexcept
		{
			if (camera_ == &camera) UnregisterCamera();
		}
		
		void UnregisterCamera() noexcept
		{
			RegisterCamera(default_camera_);
		}

		void DrawScene();

		[[nodiscard]] SharedPtr<Texture> GetTexture(Path file);
		[[nodiscard]] SharedPtr<Mesh> GetMesh(Path file);
		
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
		
		Shader sprite_shader_;
		VertexArray sprite_verts_;
		
		HashMap<Path, WeakPtr<Texture>> textures_;
		HashMap<Path, WeakPtr<Mesh>> meshes_;
		HashMap<Path, Shader> shaders_;

		ICamera* camera_ = nullptr;
		DefaultCamera default_camera_;
		
		DyArr<std::reference_wrapper<const ISprite>> sprites_;
		HashMap<Path, DyArr<std::reference_wrapper<const IMesh>>> mesh_comps_;
	};
}
