#pragma once
#include "Controller.hpp"

namespace oeng
{
inline namespace engine
{

class APlayerCameraManager;

class ENGINE_API APlayerController : public AController
{
    CLASS_BODY(APlayerController)

  public:
    [[nodiscard]] ViewInfo CalcCamera() const override;

    void SetCameraManagerClass(Name pcm_class);

    [[nodiscard]] Name GetCameraManagerClass() const noexcept
    {
        return pcm_class_;
    }

    [[nodiscard]] auto& GetCameraManager() const noexcept
    {
        return pcm_;
    }

  protected:
    void OnBeginPlay() override;

  private:
    WeakPtr<APlayerCameraManager> pcm_;
    Name pcm_class_ = u8"APlayerCameraManager"sv;
};

} // namespace engine
} // namespace oeng
