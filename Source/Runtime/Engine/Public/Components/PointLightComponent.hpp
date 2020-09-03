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
    DELETE_CPMV(PointLightComponent);

    explicit PointLightComponent(AActor& owner, int update_order = 100);
    ~PointLightComponent();

    Float radius = 1000_f;

protected:
    void OnBeginPlay() override;
};
}
}
