#pragma once
#include "PointLightComponent.hpp"

namespace oeng
{
inline namespace engine
{
class ENGINE_API SpotLightComponent : public PointLightComponent
{
CLASS_BODY(SpotLightComponent)

public:
    DELETE_CPMV(SpotLightComponent);

    explicit SpotLightComponent(AActor& owner, int update_order = 100);
    ~SpotLightComponent();

    Float cone_angle_inner_cos = 1_f; // 0_deg
    Float cone_angle_outer_cos = 0.7193398_f; // 44_deg

protected:
    void OnBeginPlay() override;
};
}
}
