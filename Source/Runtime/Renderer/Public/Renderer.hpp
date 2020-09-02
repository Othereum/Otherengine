#pragma once
#include "RHIShader.hpp"
#include "VertexArray.hpp"
#include "Window.hpp"
#include "Interfaces/Camera.hpp"

namespace logcat
{
extern RENDERER_API const LogCategory kRenderer;
}

namespace oeng::renderer
{
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

class RENDERER_API Renderer
{
public:
    DELETE_CPMV(Renderer);

    /**
     * Returns the texture corresponding to a given path. It will be loaded from file if it isn't in the cache.
     * @param path Texture file path
     * @return Loaded texture or default texture if failed to load.
     */
    [[nodiscard]] std::shared_ptr<Texture> GetTexture(Path path);
    [[nodiscard]] std::shared_ptr<Mesh> GetMesh(Path path);
    [[nodiscard]] std::shared_ptr<RHIShader> GetShader(Path path);
    [[nodiscard]] std::shared_ptr<Material> GetMaterial(Path path);

    [[nodiscard]] Window& GetWindow() noexcept
    {
        return window_;
    }

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

    RHIShader sprite_shader_;
    VertexArray sprite_verts_;

    DefaultCamera default_camera_;

    struct
    {
        RHIShader* shader;
        Material* material;
        Texture* texture;
        Mesh* mesh;
    } prev_{};
};
}
