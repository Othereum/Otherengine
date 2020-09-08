#include "Engine/LocalPlayer.hpp"
#include "GameFramework/PlayerController.hpp"

namespace oeng
{
inline namespace engine
{
ViewInfo LocalPlayer::GetViewPoint() const noexcept
{
    if (const auto ctrl = GetController().lock())
        return ctrl->CalcCamera();

    return {};
}
}
}
