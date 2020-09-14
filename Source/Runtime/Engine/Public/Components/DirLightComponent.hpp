#pragma once
#include "SkyLightComponent.hpp"

namespace oeng
{
inline namespace engine
{
class ENGINE_API DirLightComponent : public SkyLightComponent
{
    CLASS_BODY(DirLightComponent)

  protected:
    void OnBeginPlay() override;
    void OnEndPlay() override;
};
} // namespace engine
} // namespace oeng
