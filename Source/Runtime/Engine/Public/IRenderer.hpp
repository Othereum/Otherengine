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
struct ViewInfo;

class IRenderer
{
  public:
    virtual ~IRenderer() = default;
    virtual void DrawScene(const ViewInfo& view) = 0;

    virtual void AddMesh(const MeshComponent& mesh) = 0;
    virtual void AddSprite(const SpriteComponent& sprite) = 0;
    virtual void AddDirLight(const DirLightComponent& light) = 0;
    virtual void AddSkyLight(const SkyLightComponent& light) = 0;
    virtual void AddSpotLight(const SpotLightComponent& light) = 0;
    virtual void AddPointLight(const PointLightComponent& light) = 0;

    virtual void RemoveMesh(const MeshComponent& mesh) = 0;
    virtual void RemoveSprite(const SpriteComponent& sprite) = 0;
    virtual void RemoveDirLight(const DirLightComponent& light) = 0;
    virtual void RemoveSkyLight(const SkyLightComponent& light) = 0;
    virtual void RemoveSpotLight(const SpotLightComponent& light) = 0;
    virtual void RemovePointLight(const PointLightComponent& light) = 0;
};

} // namespace engine
} // namespace oeng
