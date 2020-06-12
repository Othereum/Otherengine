#pragma once
#include "Path.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "Templates/DyArr.hpp"
#include "Interfaces/Camera.hpp"

struct SDL_Window;

namespace oeng
{
	class IMeshComponent;
	class ISpriteComponent;
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

		void RegisterSprite(const ISpriteComponent& sprite);
		void RegisterMesh(const IMeshComponent& mesh);
		void RegisterCamera(ICamera& camera) noexcept { camera_ = &camera; camera.OnScreenSizeChanged(GetScreenSize()); }
		void RegisterDirLight(const IDirLight& light) noexcept { dir_light_ = &light; }
		void RegisterSkyLight(const ISkyLight& light) noexcept { sky_light_ = &light; }
		
		void UnregisterSprite(const ISpriteComponent& sprite);
		void UnregisterMesh(const IMeshComponent& mesh);
		void UnregisterCamera(const ICamera& camera) noexcept { if (camera_ == &camera) UnregisterCamera(); }
		void UnregisterCamera() noexcept { RegisterCamera(default_camera_); }
		void UnregisterDirLight(const IDirLight& light) noexcept { if (dir_light_ == &light) UnregisterDirLight(); }
		void UnregisterDirLight() noexcept;
		void UnregisterSkyLight(const ISkyLight& light) noexcept { if (sky_light_ == &light) UnregisterSkyLight(); }
		void UnregisterSkyLight() noexcept;

		/**
		 * \brief Returns the texture corresponding to a given path. It will be loaded from file if it isn't in the cache.
		 * \param path Texture file path
		 * \return The pointer to the loaded texture or default texture if failed to load
		 */
		[[nodiscard]] SharedPtr<Texture> GetTexture(Path path);
		
		/**
		 * \brief Returns the mesh corresponding to a given path. It will be loaded from file if it isn't in the cache.
		 * \param path Mesh file path
		 * \return The pointer to the loaded mesh or default mesh if failed to load
		 */
		[[nodiscard]] SharedPtr<Mesh> GetMesh(Path path);

		/**
		 * \brief Returns the shader corresponding to a given path. It will be loaded from file if it isn't in the cache.
		 * \param path Shader file path without extenstion
		 * \return The pointer to the loaded shader or default shader if failed to load
		 */
		[[nodiscard]] SharedPtr<Shader> GetShader(Path path);
		
		/**
		 * \brief Returns the material corresponding to a given path. It will be loaded from file if it isn't in the cache.
		 * \param path Material file path
		 * \return The pointer to the loaded material or default material if failed to load
		 */
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

		template <class T>
		struct Cache
		{
			HashMap<Path, WeakPtr<T>> map;
			const SharedPtr<T> default_obj = MakeShared<T>();
		};
		
		Cache<Texture> textures_;
		Cache<Mesh> meshes_;
		Cache<Shader> shaders_;
		Cache<Material> materials_;

		ICamera* camera_ = nullptr;
		DefaultCamera default_camera_;

		const IDirLight* dir_light_ = nullptr;
		const ISkyLight* sky_light_ = nullptr;
		
		DyArr<std::reference_wrapper<const ISpriteComponent>> sprites_;
		DyArr<std::reference_wrapper<const IMeshComponent>> mesh_comps_;
	};
}
