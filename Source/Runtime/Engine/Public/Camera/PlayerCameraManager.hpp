#pragma once
#include "CameraTypes.hpp"
#include "GameFramework/Actor.hpp"

namespace oeng
{
inline namespace engine
{
class ENGINE_API APlayerCameraManager : public AActor
{
CLASS_BODY(APlayerCameraManager)

public:
    [[nodiscard]] ViewInfo CalcCamera() const override;

    std::weak_ptr<AActor> view_target;
};
}
}
