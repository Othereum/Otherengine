#pragma once
#include "DirLightComponent.hpp"

namespace oeng
{
inline namespace engine
{

class ENGINE_API PointLightComponent : public DirLightComponent
{
    CLASS_BODY(PointLightComponent)

  public:
    Float radius = 1000;

  protected:
    void OnBeginPlay() override;
    void OnEndPlay() override;
};

} // namespace engine
} // namespace oeng
