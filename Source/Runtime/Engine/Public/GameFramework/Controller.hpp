#pragma once
#include "Actor.hpp"

namespace oeng
{
inline namespace engine
{
class APawn;

class ENGINE_API AController : public AActor
{
    CLASS_BODY(AController)

  public:
    [[nodiscard]] auto& GetPawn() const noexcept
    {
        return pawn_;
    }

  protected:
    virtual void OnSetPawn()
    {
    }

  private:
    friend APawn;
    void SetPawn(WeakPtr<APawn> new_pawn);

    WeakPtr<APawn> pawn_;
};
} // namespace engine
} // namespace oeng
