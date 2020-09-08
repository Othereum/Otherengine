#pragma once
#include "CameraTypes.hpp"
#include "Components/SceneComponent.hpp"

namespace oeng
{
inline namespace engine
{
class ENGINE_API CameraComponent : public SceneComponent
{
CLASS_BODY(CameraComponent)

public:
    [[nodiscard]] ViewInfo GetCameraView() const noexcept
    {
        return {GetWorldPos(), GetForward(), vfov, near, far};
    }

    Rad vfov = 60_deg;
    Float near = 10;
    Float far = 10000;
};
}
}
