#include "Camera/PlayerCameraManager.hpp"
#include "GameFramework/Actor.hpp"

namespace oeng
{
inline namespace engine
{
ViewInfo APlayerCameraManager::CalcCamera() const
{
    if (const auto target = view_target.lock())
        return target->CalcCamera();

    return AActor::CalcCamera();
}
}
}
