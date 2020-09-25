#include "GameFramework/Pawn.hpp"
#include "GameFramework/Controller.hpp"

namespace oeng
{
inline namespace engine
{
void APawn::OnSetOwner()
{
    if (const auto controller = GetController())
    {
        controller->SetPawn(weak_from_this());
    }
}

SharedPtr<AController> APawn::GetController() const noexcept
{
    return CastChecked<AController>(GetOwner().lock());
}
} // namespace engnie
} // namespace oeng
