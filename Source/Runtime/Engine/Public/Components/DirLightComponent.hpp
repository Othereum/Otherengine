#pragma once
#include "SkyLightComponent.hpp"

namespace oeng
{
inline namespace engine
{
class ENGINE_API DirLightComponent : public SkyLightComponent
{
CLASS_BODY(DirLightComponent)

public:
    DELETE_CPMV(DirLightComponent);

    explicit DirLightComponent(AActor& owner, int update_order = 100);
    ~DirLightComponent();

protected:
    void OnBeginPlay() override;
};
}
}
