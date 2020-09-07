#pragma once
#include "Controller.hpp"

namespace oeng
{
inline namespace engine
{
class APlayerCameraManager;

class APlayerController : public AController
{
CLASS_BODY(APlayerController)

public:
    [[nodiscard]] auto& GetCameraManager() const noexcept
    {
        return pcm_;
    }

private:
    std::weak_ptr<APlayerCameraManager> pcm_;
};
}
}
