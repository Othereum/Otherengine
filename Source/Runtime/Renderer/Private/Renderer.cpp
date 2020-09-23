#include "Renderer.hpp"
#include "Camera/CameraTypes.hpp"
#include "Components/MeshComponent.hpp"
#include "Components/SpotLightComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "DynamicRHI.hpp"
#include "Engine/AssetManager.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Mesh.hpp"
#include "Engine/Texture.hpp"
#include "Materials/IMaterial.hpp"
#include "RHIMesh.hpp"
#include "RHIShader.hpp"
#include "RHIWindow.hpp"
#include <SDL2/SDL.h>

namespace logcat
{
const LogCategory kRenderer{u8"Renderer"sv};
}

namespace oeng
{
inline namespace renderer
{
Renderer::Renderer(Engine& engine) : engine_{engine}
{
    sprite_mat_ = AssetManager::Get().Load<IMaterial>(u8"../Engine/Assets/M_Sprite.json"sv);
    sprite_mat_->GetRHI().ApplyParam(u8"uViewProj"sv, MakeSimpleViewProj<4>(engine.GetWindow().GetSize()));

    sprite_mesh_ = AssetManager::Get().Load<Mesh>(u8"../Engine/Assets/SM_Sprite.json"sv);

    static DirLightComponent default_light;
    default_light.color = {};
    dir_lights_.emplace_back(default_light);
    sky_lights_.emplace_back(default_light);
}

Renderer::~Renderer() = default;

void Renderer::PreDrawScene() const
{
    SCOPE_STACK_COUNTER(PreDrawScene);
    engine_.GetWindow().ClearBuffer();
}

void Renderer::DrawScene(const ViewInfo& view)
{
    SCOPE_STACK_COUNTER(DrawScene);
    PreDrawScene();

    if (auto view_m = MakeLookAt(view.origin, view.direction, UVec3::up))
    {
        auto proj_m = MakePerspective(Vec2{engine_.GetWindow().GetSize()}, view.near, view.far, view.vfov);
        view_proj = *view_m * proj_m;
        view_origin = view.origin;
    }

    Draw3D();
    Draw2D();
    PostDrawScene();
}

void Renderer::PostDrawScene() const
{
    SCOPE_STACK_COUNTER(PostDrawScene);
    engine_.GetWindow().SwapBuffer();
}

void Renderer::Draw3D()
{
    DynamicRHI::Get().PreDraw3D();
    prev_ = {};

    for (auto mesh_comp : meshes_)
    {
        DrawMesh(mesh_comp);
    }
}

void Renderer::Draw2D()
{
    DynamicRHI::Get().PreDraw2D();
    sprite_mat_->GetRHI().Activate();
    sprite_mesh_->GetRHI().Activate();

    for (auto sprite_ref : sprites_)
    {
        const auto& sprite = sprite_ref.get();
        if (!sprite.IsActive())
            continue;

        SCOPE_STACK_COUNTER(DrawSprite);
        sprite_mat_->GetRHI().ApplyParam(u8"uWorldTransform"sv, sprite.GetWorldTrsfMatrix());
        sprite_mesh_->GetRHI().Draw();
    }
}

void Renderer::DrawMesh(const MeshComponent& mesh_comp)
{
    if (!ShouldDraw(mesh_comp))
        return;

    SCOPE_STACK_COUNTER(DrawMesh);

    auto material = mesh_comp.GetMaterial();
    auto& shader = material->GetRHI();
    if (&shader != prev_.shader)
    {
        shader.Activate();
        shader.ApplyParam(u8"uViewProj"sv, view_proj);
        shader.ApplyParam(u8"uCamPos"sv, view_origin);

        auto& dir_light = dir_lights_.back().get();
        shader.ApplyParam(u8"uDirLight.dir"sv, dir_light.GetForward());
        shader.ApplyParam(u8"uDirLight.color"sv, dir_light.color);

        auto& sky_light = sky_lights_.back().get();
        shader.ApplyParam(u8"uSkyLight"sv, sky_light.color);

        prev_.shader = &shader;
    }

    if (material.get() != prev_.material)
    {
        material->ApplyParams();
        prev_.material = material.get();
    }

    auto mesh = mesh_comp.GetMesh();
    auto& verts = mesh->GetRHI();
    if (mesh.get() != prev_.mesh)
    {
        verts.Activate();
        prev_.mesh = mesh.get();
    }

    DrawPointLights(mesh_comp);
    DrawSpotLights(mesh_comp);

    shader.ApplyParam(u8"uWorldTransform"sv, mesh_comp.GetWorldTrsfMatrix());
    verts.Draw();
}

template <class Lights, class Fn>
static void DrawLights(std::u8string_view name, const Lights& lights, const MeshComponent& mesh_comp,
                       Fn&& apply_extra_params)
{
    const Name num_lights = fmt::format(u8"uNum{}Lights"sv, name);
    auto& shader = mesh_comp.GetMaterial()->GetRHI();
    if (!shader.IsScalarParam(num_lights))
        return;

    static constexpr auto max_lights = 4;
    const auto mesh_radius = mesh_comp.GetScaledRadius();
    auto idx = 0;

    for (auto light_ref : lights)
    {
        if (idx >= max_lights)
            break;

        const auto& light = light_ref.get();
        if (!light.IsActive())
            continue;

        if (!IsOverlapped({light.GetWorldPos(), light.radius}, {mesh_comp.GetWorldPos(), mesh_radius}))
            continue;

        SCOPE_STACK_COUNTER(DrawLight);

        auto apply_param = [&](std::u8string_view param, auto&& value) {
            return shader.ApplyParam(fmt::format(u8"u{}Lights[{}].{}"sv, name, idx, param), value);
        };

        apply_param(u8"color"sv, light.color);
        apply_param(u8"pos"sv, light.GetWorldPos());
        apply_param(u8"radius"sv, light.radius);
        apply_extra_params(apply_param, light);

        ++idx;
    }

    shader.ApplyParam(num_lights, idx);
}

void Renderer::DrawPointLights(const MeshComponent& mesh_comp) const
{
    DrawLights(u8"Point"sv, point_lights_, mesh_comp, [](auto&&...) {});
}

void Renderer::DrawSpotLights(const MeshComponent& mesh_comp) const
{
    DrawLights(u8"Spot"sv, spot_lights_, mesh_comp, [](auto&& apply_param, const SpotLightComponent& light) {
        apply_param(u8"dir"sv, light.GetForward());
        apply_param(u8"inner"sv, light.cone_angle_inner_cos);
        apply_param(u8"outer"sv, light.cone_angle_outer_cos);
    });
}

bool Renderer::ShouldDraw(const MeshComponent& mesh_comp) const noexcept
{
    if (!mesh_comp.IsActive())
        return false;

    const Sphere mesh_sphere{mesh_comp.GetWorldPos(), mesh_comp.GetScaledRadius()};
    const Sphere camera_sphere{view_origin, mesh_comp.max_draw_dist};
    if (!IsOverlapped(mesh_sphere, camera_sphere))
        return false;

    return true;
}

template <class T, class Compare> static void AddComp(Renderer::CompArr<T>& arr, const T& obj, Compare cmp)
{
    arr.emplace(std::upper_bound(arr.begin(), arr.end(), obj, cmp), obj);
}

template <class T> void RemoveComp(Renderer::CompArr<T>& arr, const T& obj)
{
    const auto cmp = [&obj](const T& x) { return &x == &obj; };
    const auto found = std::find_if(arr.rbegin(), arr.rend(), cmp);
    if (found != arr.rend())
        arr.erase(found.base() - 1);
}

void Renderer::AddMesh(const MeshComponent& mesh)
{
    // Group mesh components in order of importance for [Shader -> Material -> Mesh]
    AddComp(meshes_, mesh, [](const MeshComponent& a, const MeshComponent& b) {
        auto mat1 = a.GetMaterial(), mat2 = b.GetMaterial();
        auto s1 = &mat1->GetRHI(), s2 = &mat2->GetRHI();

        if (s1 != s2)
            return s1 < s2;

        if (mat1 != mat2)
            return mat1 < mat2;

        return a.GetMesh() < b.GetMesh();
    });
}

void Renderer::AddSprite(const SpriteComponent& sprite)
{
    AddComp(sprites_, sprite,
            [](const SpriteComponent& a, const SpriteComponent& b) { return a.GetDrawOrder() < b.GetDrawOrder(); });
}

void Renderer::AddDirLight(const DirLightComponent& light)
{
    dir_lights_.emplace_back(light);
}

void Renderer::AddSkyLight(const SkyLightComponent& light)
{
    sky_lights_.emplace_back(light);
}

void Renderer::AddSpotLight(const SpotLightComponent& light)
{
    spot_lights_.emplace_back(light);
}

void Renderer::AddPointLight(const PointLightComponent& light)
{
    point_lights_.emplace_back(light);
}

void Renderer::RemoveSprite(const SpriteComponent& sprite)
{
    RemoveComp(sprites_, sprite);
}

void Renderer::RemoveMesh(const MeshComponent& mesh)
{
    RemoveComp(meshes_, mesh);
}

void Renderer::RemoveDirLight(const DirLightComponent& light)
{
    RemoveComp(dir_lights_, light);
}

void Renderer::RemoveSkyLight(const SkyLightComponent& light)
{
    RemoveComp(sky_lights_, light);
}

void Renderer::RemoveSpotLight(const SpotLightComponent& light)
{
    RemoveComp(spot_lights_, light);
}

void Renderer::RemovePointLight(const PointLightComponent& light)
{
    RemoveComp(point_lights_, light);
}

} // namespace renderer
} // namespace oeng
