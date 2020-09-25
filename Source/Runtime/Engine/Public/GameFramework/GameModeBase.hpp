#pragma once
#include "Actor.hpp"

namespace logcat
{
extern ENGINE_API const LogCategory kGameMode;
}

namespace oeng
{
inline namespace engine
{

class Player;
class AController;
class APlayerController;

class ENGINE_API AGameModeBase : public AActor
{
    CLASS_BODY(AGameModeBase)

  public:
    AGameModeBase();
    virtual APlayerController& Login(Player& new_player);
    virtual void HandleStartingNewPlayer(APlayerController& new_player);
    virtual void RestartPlayer(AController& player);
    void SpawnDefaultPawnFor(AController& player);

  protected:
    Name pc_class_ = u8"APlayerController"sv;
    Name default_pawn_class_ = u8"APawn"sv;
};

} // namespace engine
} // namespace oeng
