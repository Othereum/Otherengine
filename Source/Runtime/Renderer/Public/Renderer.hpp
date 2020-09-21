#pragma once
#include "IRenderer.hpp"

namespace logcat
{
extern RENDERER_API const LogCategory kRenderer;
}

namespace oeng
{
inline namespace engine
{
class IMaterial;
class Texture;
class Mesh;
class Engine;
} // namespace engine

inline namespace rhi
{
class RHIShader;
}

inline namespace renderer
{
class RENDERER_API Renderer final : public IRenderer
{
  public:
    explicit Renderer(Engine& engine);
    ~Renderer();

    void DrawScene(const ViewInfo& view) override;

    void AddMesh(const MeshComponent& mesh) override;
    void AddSprite(const SpriteComponent& sprite) override;
    void AddDirLight(const DirLightComponent& light) override;
    void AddSkyLight(const SkyLightComponent& light) override;
    void AddSpotLight(const SpotLightComponent& light) override;
    void AddPointLight(const PointLightComponent& light) override;

    void RemoveMesh(const MeshComponent& mesh) override;
    void RemoveSprite(const SpriteComponent& sprite) override;
    void RemoveDirLight(const DirLightComponent& light) override;
    void RemoveSkyLight(const SkyLightComponent& light) override;
    void RemoveSpotLight(const SpotLightComponent& light) override;
    void RemovePointLight(const PointLightComponent& light) override;

  private:
    void PreDrawScene() const;
    void PostDrawScene() const;

    void Draw3D();
    void Draw2D();
    void DrawMesh(const MeshComponent& mesh_comp);
    void DrawPointLights(const MeshComponent& mesh_comp) const;
    void DrawSpotLights(const MeshComponent& mesh_comp) const;
    [[nodiscard]] bool ShouldDraw(const MeshComponent& mesh_comp) const noexcept;

    Engine& engine_;

    struct
    {
        RHIShader* shader;
        IMaterial* material;
        Mesh* mesh;
    } prev_{};

    template <class T> using CompArr = std::vector<std::reference_wrapper<const T>>;

    CompArr<MeshComponent> meshes_;
    CompArr<SpriteComponent> sprites_;
    CompArr<DirLightComponent> dir_lights_;
    CompArr<SkyLightComponent> sky_lights_;
    CompArr<SpotLightComponent> spot_lights_;
    CompArr<PointLightComponent> point_lights_;

    SharedPtr<IMaterial> sprite_mat_;
    SharedPtr<Mesh> sprite_mesh_;

    Mat4 view_proj = Mat4::Identity();
    Vec3 view_origin;
};
} // namespace renderer
} // namespace oeng
