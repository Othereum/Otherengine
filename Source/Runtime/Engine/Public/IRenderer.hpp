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

class ENGINE_API IRenderer
{
INTERFACE_BODY(IRenderer)

public:
    [[nodiscard]] static IRenderer& Get() noexcept;

    virtual void AddSprite(SpriteComponent& sprite) = 0;
    virtual void AddMesh(MeshComponent& mesh) = 0;
    virtual void AddDirLight(DirLightComponent& light) = 0;
    virtual void AddSkyLight(SkyLightComponent& light) = 0;
    virtual void AddPointLight(PointLightComponent& light) = 0;
    virtual void AddSpotLight(SpotLightComponent& light) = 0;

    virtual void RemoveSprite(SpriteComponent& sprite) = 0;
    virtual void RemoveMesh(MeshComponent& mesh) = 0;
    virtual void RemoveDirLight(DirLightComponent& light) = 0;
    virtual void RemoveSkyLight(SkyLightComponent& light) = 0;
    virtual void RemovePointLight(PointLightComponent& light) = 0;
    virtual void RemoveSpotLight(SpotLightComponent& light) = 0;
};
}
}
