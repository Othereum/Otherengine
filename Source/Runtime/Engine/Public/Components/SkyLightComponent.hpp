#pragma once
#include "SceneComponent.hpp"

namespace oeng
{
inline namespace engine
{
class ENGINE_API SkyLightComponent : public SceneComponent
{
    CLASS_BODY(SkyLightComponent)

  public:
    Vec3 color{All{}, 0.2};

  protected:
    void OnBeginPlay() override;
    void OnEndPlay() override;
};

} // namespace engine
} // namespace oeng
