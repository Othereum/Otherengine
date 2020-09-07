#pragma once
#include "GameFramework/Actor.hpp"

namespace oeng
{
inline namespace engine
{
class ENGINE_API APlayerCameraManager : public AActor
{
CLASS_BODY(APlayerCameraManager)

public:

private:
    std::weak_ptr<AActor> view_target_;
};
}
}
