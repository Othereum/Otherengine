#pragma once

namespace oeng
{
inline namespace engine
{
class SpriteComponent;
class MeshComponent;
class DirLightComponent;
class SkyLightComponent;
class PointLightComponent;
class SpotLightComponent;
class CameraComponent;

class ENGINE_API IRenderer
{
INTERFACE_BODY(IRenderer)

public:
    void AddSprite(SpriteComponent& sprite);
    void AddMesh(MeshComponent& mesh);
    void AddDirLight(DirLightComponent& light);
    void AddSkyLight(SkyLightComponent& light);
    void AddPointLight(PointLightComponent& light);
    void AddSpotLight(SpotLightComponent& light);

    void RemoveSprite(SpriteComponent& sprite);
    void RemoveMesh(MeshComponent& mesh);
    void RemoveDirLight(DirLightComponent& light);
    void RemoveSkyLight(SkyLightComponent& light);
    void RemovePointLight(PointLightComponent& light);
    void RemoveSpotLight(SpotLightComponent& light);

    template <class T>
    using CompArr = std::vector<T*>;

private:
    CompArr<DirLightComponent> dir_lights_;
    CompArr<SkyLightComponent> sky_lights_;
    CompArr<PointLightComponent> point_lights_;
    CompArr<SpotLightComponent> spot_lights_;
    CompArr<SpriteComponent> sprites_;
    CompArr<MeshComponent> meshes_;
};
}
}
