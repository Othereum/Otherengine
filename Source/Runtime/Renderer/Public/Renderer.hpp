#pragma once
#include "IRenderer.hpp"
#include "Window.hpp"

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

    [[nodiscard]] Window& GetWindow() noexcept
    {
        return window_;
    }

    void DrawScene(const ViewInfo& view) override;

    void AddMesh(MeshComponent& mesh) override;
    void AddSprite(SpriteComponent& sprite) override;
    void AddDirLight(DirLightComponent& light) override;
    void AddSkyLight(SkyLightComponent& light) override;
    void AddSpotLight(SpotLightComponent& light) override;
    void AddPointLight(PointLightComponent& light) override;

    void RemoveMesh(MeshComponent& mesh) override;
    void RemoveSprite(SpriteComponent& sprite) override;
    void RemoveDirLight(DirLightComponent& light) override;
    void RemoveSkyLight(SkyLightComponent& light) override;
    void RemoveSpotLight(SpotLightComponent& light) override;
    void RemovePointLight(PointLightComponent& light) override;

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
    Window window_;

    struct
    {
        RHIShader* shader;
        IMaterial* material;
        Texture* texture;
        Mesh* mesh;
    } prev_{};

    std::vector<std::reference_wrapper<MeshComponent>> meshes_;
    std::vector<std::reference_wrapper<SpriteComponent>> sprites_;
    std::vector<std::reference_wrapper<DirLightComponent>> dir_lights_;
    std::vector<std::reference_wrapper<SkyLightComponent>> sky_lights_;
    std::vector<std::reference_wrapper<SpotLightComponent>> spot_lights_;
    std::vector<std::reference_wrapper<PointLightComponent>> point_lights_;

    SharedPtr<IMaterial> sprite_mat_;
    SharedPtr<Mesh> sprite_mesh_;

    Mat4 view_proj = Mat4::Identity();
    Vec3 view_origin;
};
} // namespace renderer
} // namespace oeng
