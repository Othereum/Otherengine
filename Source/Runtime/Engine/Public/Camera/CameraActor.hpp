#pragma once
#include "GameFramework/Actor.hpp"

namespace oeng
{
inline namespace engine
{
class CameraComponent;

class ENGINE_API ACameraActor : public AActor
{
    CLASS_BODY(ACameraActor)

  public:
    ACameraActor();
    [[nodiscard]] ViewInfo CalcCamera() const override;

    void SetVFov(Rad vfov) const noexcept;
    void SetNearFar(Float near, Float far) const noexcept;

    [[nodiscard]] Rad GetVFov() const noexcept;
    [[nodiscard]] Float GetNear() const noexcept;
    [[nodiscard]] Float GetFar() const noexcept;

    CameraComponent& camera_;
};
} // namespace engine
} // namespace oeng
