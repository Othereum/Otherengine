#include "GameFramework/PlayerController.hpp"
#include "Camera/PlayerCameraManager.hpp"
#include "Engine/World.hpp"

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
    GetWorld()->SpawnActor<>()
}
}
}
