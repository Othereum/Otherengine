#pragma once
#include "Player.hpp"

namespace oeng
{
inline namespace engine
{
struct ViewInfo
{
    Mat4 view_proj;
    Vec3 origin;
};

class LocalPlayer : public Player
{
public:
    [[nodiscard]] ViewInfo CalcViewInfo() const noexcept;
};
}
}
