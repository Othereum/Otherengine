#include "GameFramework/GameModeBase.hpp"
#include "Engine/Player.hpp"
#include "Engine/World.hpp"
#include "GameFramework/PlayerController.hpp"

namespace logcat
{
const LogCategory kGameMode{u8"GameMode"sv};
}

namespace oeng
{
inline namespace engine
{

AGameModeBase::AGameModeBase()
{
    immortal = true;
}

APlayerController& AGameModeBase::Login(Player& new_player)
{
    APlayerController* pc;

    try
    {
        pc = &GetWorld()->SpawnActor<APlayerController>(pc_class_);
    }
    catch (const std::exception& e)
    {
        OE_LOG(kGameMode, kErr, u8"Failed to create player controller '{}': {}"sv, *pc_class_, AsString8(e.what()));
        OE_LOG(kGameMode, kErr, u8"Falling back to default player controller..."sv);
        pc = &GetWorld()->SpawnActor<APlayerController>();
    }

    new_player.SwitchController(pc->weak_from_this());
    return *pc;
}

void AGameModeBase::HandleStartingNewPlayer(APlayerController& new_player)
{
    RestartPlayer(new_player);
}

void AGameModeBase::RestartPlayer(AController& player)
{
    SpawnDefaultPawnFor(player);
}

void AGameModeBase::SpawnDefaultPawnFor(AController& player)
{

}

} // namespace engine
} // namespace oeng
