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
    Vec3 color{1, 1, 1};

protected:
    void OnBeginPlay() override;
    void OnEndPlay() override;
};
}
}
