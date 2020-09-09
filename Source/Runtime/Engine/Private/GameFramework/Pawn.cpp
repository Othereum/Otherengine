#include "GameFramework/Pawn.hpp"
#include "GameFramework/Controller.hpp"

namespace oeng
{
inline namespace engine
{

SharedPtr<AController> APawn::GetController() const noexcept
{
    return CastChecked<AController>(GetOwner().lock());
}

} // namespace engnie
} // namespace oeng
