#pragma once
#include "Actor.hpp"

namespace oeng
{
inline namespace engine
{

class APlayerController;

class ENGINE_API AGameModeBase : public AActor
{
    CLASS_BODY(AGameModeBase)

  public:
    virtual APlayerController& Login();
};

} // namespace engine
} // namespace oeng
