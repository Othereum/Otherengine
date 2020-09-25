#pragma once
#include "Actor.hpp"

namespace oeng
{
inline namespace engine
{

class AController;

class ENGINE_API APawn : public AActor
{
    CLASS_BODY(APawn)

  public:
    void OnSetOwner() override;
    [[nodiscard]] SharedPtr<AController> GetController() const noexcept;
};

} // namespace engine
} // namespace oeng
