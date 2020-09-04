#pragma once
#include "LocalPlayer.hpp"
#include "World.hpp"

namespace oeng
{
inline namespace engine
{
class GameInstance
{
public:
    LocalPlayer local_player;
    World world_;
};
}
}
