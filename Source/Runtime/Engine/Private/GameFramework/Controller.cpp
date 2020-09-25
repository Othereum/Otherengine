#include "GameFramework/Controller.hpp"

namespace oeng
{
inline namespace engine
{
void AController::SetPawn(WeakPtr<APawn> new_pawn)
{
    pawn_ = std::move(new_pawn);
    OnSetPawn();
}
} // namespace engine
} // namespace oeng
