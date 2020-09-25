#pragma once
#include "SkyLightComponent.hpp"

namespace oeng
{
inline namespace engine
{
class ENGINE_API DirLightComponent : public SceneComponent
{
    CLASS_BODY(DirLightComponent)

  public:
    Vec3 color{1, 1, 1};

  protected:
    void OnBeginPlay() override;
    void OnEndPlay() override;
};
} // namespace engine
} // namespace oeng
