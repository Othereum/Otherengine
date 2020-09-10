#include "Camera/CameraActor.hpp"
#include "Camera/CameraComponent.hpp"

namespace oeng
{
inline namespace engine
{
ACameraActor::ACameraActor() : camera_{AddComponent<CameraComponent>()}
{
    SetRootComponent(&camera_);
}

ViewInfo ACameraActor::CalcCamera() const
{
    return camera_.GetCameraView();
}

void ACameraActor::SetVFov(Rad vfov) const noexcept
{
    camera_.vfov = vfov;
}

void ACameraActor::SetNearFar(Float near, Float far) const noexcept
{
    camera_.near = near;
    camera_.far = far;
}

Rad ACameraActor::GetVFov() const noexcept
{
    return camera_.vfov;
}

Float ACameraActor::GetNear() const noexcept
{
    return camera_.near;
}

Float ACameraActor::GetFar() const noexcept
{
    return camera_.far;
}
} // namespace engine
} // namespace oeng
