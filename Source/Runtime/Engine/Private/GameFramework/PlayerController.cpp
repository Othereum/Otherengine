#include "GameFramework/PlayerController.hpp"
#include "Camera/PlayerCameraManager.hpp"
#include "Engine/World.hpp"
#include "GameFramework/Pawn.hpp"

namespace oeng
{
inline namespace engine
{
ViewInfo APlayerController::CalcCamera() const
{
    if (const auto pcm = pcm_.lock())
        return pcm->CalcCamera();

    return AController::CalcCamera();
}

void APlayerController::SetCameraManagerClass(Name pcm_class)
{
    if (ENSURE(!HasBegunPlay()))
    {
        pcm_class_ = pcm_class;
    }
}

void APlayerController::OnBeginPlay()
{
    auto& pcm = GetWorld().SpawnActor<APlayerCameraManager>(pcm_class_);
    pcm_ = pcm.weak_from_this();
    pcm.view_target = GetPawn();
}

void APlayerController::OnSetPawn()
{
    if (const auto pcm = pcm_.lock())
        pcm->view_target = GetPawn();
}
} // namespace engine
} // namespace oeng
