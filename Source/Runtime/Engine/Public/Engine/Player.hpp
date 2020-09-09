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

    void SwitchController(WeakPtr<APlayerController> new_pc)
    {
        controller_ = std::move(new_pc);
    }

private:
    WeakPtr<APlayerController> controller_;
};
}
}
