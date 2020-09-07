#pragma once

namespace oeng
{
inline namespace engine
{
class APlayerController;

class Player
{
public:
    [[nodiscard]] auto& GetController() const noexcept
    {
        return controller_;
    }

private:
    std::weak_ptr<APlayerController> controller_;
};
}
}
