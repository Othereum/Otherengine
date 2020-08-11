#pragma once
#include "Path.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "Window.hpp"
#include "Interfaces/Camera.hpp"
#include "Templates/DyArr.hpp"
#include "Templates/Pointer.hpp"

namespace logcat
{
	extern RENDERER_API const LogCategory kRenderer;
}

namespace oeng::engine
{
	class Engine;
}

namespace oeng::renderer
{
	class ISpriteComponent;
	class IMeshComponent;
	class IDirLight;
	class ISkyLight;
	class IPointLight;
	class ISpotLight;
	class Texture;
	class Mesh;
	class Material;
	
	class DefaultCamera final : public ICamera
	{
	public:
		[[nodiscard]] const Vec3& GetPos() const noexcept override;
		[[nodiscard]] const Mat4& GetViewProj() const noexcept override;
		[[nodiscard]] const Data& GetData() const noexcept override;
		void OnScreenSizeChanged(Vec2u16 scr) override;

	private:
		Mat4 view_proj_;
	};
	
	template <class T>
	struct AssetCache
	{
		template <class... Args>
		AssetCache(Args&&... args) :default_obj{ MakeShared<T>(std::forward<Args>(args)...) } {}

		HashMap<Path, WeakPtr<T>> map;
		const SharedRef<T> default_obj;
	};

	class RENDERER_API Renderer
	{
	public:
		DELETE_CPMV(Renderer);

		void RegisterSprite(const ISpriteComponent& sprite);
		void RegisterMesh(const IMeshComponent& mesh);
		void RegisterCamera(ICamera& camera) noexcept;
		void RegisterDirLight(const IDirLight& light) noexcept { dir_light_ = &light; }
		void RegisterSkyLight(const ISkyLight& light) noexcept { sky_light_ = &light; }
		void RegisterPointLight(const IPointLight& light) { point_lights_.emplace_back(light); }
		void RegisterSpotLight(const ISpotLight& light) { spot_lights_.emplace_back(light); }
		
		void UnregisterSprite(const ISpriteComponent& sprite);
		void UnregisterMesh(const IMeshComponent& mesh);
		void UnregisterCamera(const ICamera& camera) noexcept { if (camera_ == &camera) UnregisterCamera(); }
		void UnregisterCamera() noexcept { RegisterCamera(default_camera_); }
		void UnregisterDirLight(const IDirLight& light) noexcept { if (dir_light_ == &light) UnregisterDirLight(); }
		void UnregisterDirLight() noexcept;
		void UnregisterSkyLight(const ISkyLight& light) noexcept { if (sky_light_ == &light) UnregisterSkyLight(); }
		void UnregisterSkyLight() noexcept;
		void UnregisterPointLight(const IPointLight& light);
		void UnregisterSpotLight(const ISpotLight& light);

		[[nodiscard]] bool IsCameraRegistered() const noexcept { return camera_ == &default_camera_; }
		[[nodiscard]] bool IsCameraRegistered(const ICamera& camera) const noexcept { return camera_ == &camera; }
		[[nodiscard]] bool IsDirLightRegistered() const noexcept;
		[[nodiscard]] bool IsDirLightRegistered(const IDirLight& light) const noexcept { return dir_light_ == &light; }
		[[nodiscard]] bool IsSkyLightRegistered() const noexcept;
		[[nodiscard]] bool IsSkyLightRegistered(const ISkyLight& light) const noexcept { return sky_light_ == &light; }

		/**
		 * Returns the texture corresponding to a given path. It will be loaded from file if it isn't in the cache.
		 * @param path Texture file path
		 * @return Loaded texture or default texture if failed to load.
		 */
		[[nodiscard]] SharedRef<Texture> GetTexture(Path path);
		[[nodiscard]] SharedRef<Mesh> GetMesh(Path path);
		[[nodiscard]] SharedRef<Shader> GetShader(Path path);
		[[nodiscard]] SharedRef<Material> GetMaterial(Path path);
		[[nodiscard]] SharedRef<Texture> GetDefaultTexture() const noexcept { return textures_.default_obj; }
		[[nodiscard]] SharedRef<Mesh> GetDefaultMesh() const noexcept { return meshes_.default_obj; }
		[[nodiscard]] SharedRef<Shader> GetDefaultShader() const noexcept { return shaders_.default_obj; }
		[[nodiscard]] SharedRef<Material> GetDefaultMaterial() const noexcept { return materials_.default_obj; }
		
		[[nodiscard]] Window& GetWindow() noexcept { return window_; }

		template <class T>
		using CompArr = DyArr<std::reference_wrapper<const T>>;

	private:
		friend engine::Engine;
		
		Renderer();
		~Renderer();

		void PreDrawScene() const;
		void DrawScene();
		void PostDrawScene() const;

		void Draw3D();
		void Draw2D();
		void DrawMesh(const IMeshComponent& mesh_comp);
		void DrawPointLights(const IMeshComponent& mesh_comp) const;
		void DrawSpotLights(const IMeshComponent& mesh_comp) const;
		[[nodiscard]] bool ShouldDraw(const IMeshComponent& mesh_comp) const noexcept;

		Window window_;
		
		Shader sprite_shader_;
		VertexArray sprite_verts_;

		AssetCache<Texture> textures_;
		AssetCache<Shader> shaders_;
		AssetCache<Material> materials_;
		AssetCache<Mesh> meshes_;

		ICamera* camera_{};
		DefaultCamera default_camera_;

		const IDirLight* dir_light_{};
		const ISkyLight* sky_light_{};

		CompArr<IPointLight> point_lights_;
		CompArr<ISpotLight> spot_lights_;
		
		CompArr<ISpriteComponent> sprites_;
		CompArr<IMeshComponent> mesh_comps_;

		struct
		{
			Shader* shader;
			Material* material;
			Texture* texture;
			Mesh* mesh;
		} prev_{};
	};
}
