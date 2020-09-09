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

    void SwitchController(std::weak_ptr<APlayerController> new_pc)
    {
        controller_ = std::move(new_pc);
    }

private:
    std::weak_ptr<APlayerController> controller_;
};
}
}
