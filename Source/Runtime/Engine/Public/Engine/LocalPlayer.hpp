#pragma once
#include "Player.hpp"
#include "Camera/CameraTypes.hpp"

namespace oeng
{
inline namespace engine
{
class ENGINE_API LocalPlayer : public Player
{
public:
    [[nodiscard]] ViewInfo GetViewPoint() const noexcept;
};
}
}
