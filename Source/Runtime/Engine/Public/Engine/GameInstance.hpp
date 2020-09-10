#pragma once
#include "LocalPlayer.hpp"
#include "World.hpp"

namespace oeng
{
inline namespace engine
{

class Engine;

class GameInstance
{
  public:
    GameInstance(Engine& engine) : engine{engine}, world{*this}
    {
    }

    Engine& engine;
    LocalPlayer local_player;
    World world;
};

} // namespace engine
} // namespace oeng
