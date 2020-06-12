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
	class IDirLight;
	class ISkyLight;
	class Texture;
	class Mesh;
	class Material;

	using WindowPtr = UniquePtr<SDL_Window, void(*)(SDL_Window*)>;
	using GlContextPtr = UniquePtr<void, void(*)(void*)>;

	class DefaultCamera final : public ICamera
	{
	public:
		[[nodiscard]] const Vec3& GetPos() const noexcept override;
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
		
		void DrawScene();

		void RegisterSprite(const ISprite& sprite);
		void RegisterMesh(const IMesh& mesh);
		void RegisterCamera(ICamera& camera) noexcept { camera_ = &camera; camera.OnScreenSizeChanged(GetScreenSize()); }
		void RegisterDirLight(const IDirLight& light) noexcept { dir_light_ = &light; }
		void RegisterSkyLight(const ISkyLight& light) noexcept { sky_light_ = &light; }
		
		void UnregisterSprite(const ISprite& sprite);
		void UnregisterMesh(const IMesh& mesh);
		void UnregisterCamera(const ICamera& camera) noexcept { if (camera_ == &camera) UnregisterCamera(); }
		void UnregisterCamera() noexcept { RegisterCamera(default_camera_); }
		void UnregisterDirLight(const IDirLight& light) noexcept { if (dir_light_ == &light) UnregisterDirLight(); }
		void UnregisterDirLight() noexcept;
		void UnregisterSkyLight(const ISkyLight& light) noexcept { if (sky_light_ == &light) UnregisterSkyLight(); }
		void UnregisterSkyLight() noexcept;

		[[nodiscard]] SharedPtr<Texture> GetTexture(Path path);
		[[nodiscard]] SharedPtr<Mesh> GetMesh(Path path);
		[[nodiscard]] SharedPtr<Shader> GetShader(Path path);
		[[nodiscard]] SharedPtr<Material> GetMaterial(Path path);
		
		[[nodiscard]] Vec2u16 GetScreenSize() const noexcept { return scr_sz_; }
		[[nodiscard]] IEngine& GetEngine() const noexcept { return engine_; }

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;

	private:
		void Draw3D();
		void Draw2D();
		
		IEngine& engine_;

		Vec2u16 scr_sz_;
		WindowPtr window_;
		GlContextPtr gl_context_;
		
		Shader sprite_shader_;
		VertexArray sprite_verts_;
		
		HashMap<Path, WeakPtr<Texture>> textures_;
		HashMap<Path, WeakPtr<Mesh>> meshes_;
		HashMap<Path, WeakPtr<Shader>> shaders_;
		HashMap<Path, WeakPtr<Material>> materials_;

		ICamera* camera_ = nullptr;
		DefaultCamera default_camera_;

		const IDirLight* dir_light_ = nullptr;
		const ISkyLight* sky_light_ = nullptr;
		
		DyArr<std::reference_wrapper<const ISprite>> sprites_;
		DyArr<std::reference_wrapper<const IMesh>> mesh_comps_;
	};
}
