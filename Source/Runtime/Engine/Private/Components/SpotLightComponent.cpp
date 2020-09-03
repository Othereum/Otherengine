#include "Components/SpotLightComponent.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"

namespace oeng
{
inline namespace engine
{
	SpotLightComponent::SpotLightComponent(AActor& owner, int update_order)
		:SceneComponent{owner, update_order},
		data_{GetWorldPos(), Vec3::one, GetForward(), 1000, {0_deg, 44_deg}}
	{
	}

	SpotLightComponent::~SpotLightComponent()
	{
		GetRenderer().UnregisterSpotLight(*this);
	}

	bool SpotLightComponent::ShouldAffect() const noexcept
	{
		return IsActive()
			& (data_.radius > kSmallNum)
			& (data_.color.LenSqr() > kSmallNum)
			& !IsNearlyEqual(data_.angle_cos.outer, 1);
	}

	void SpotLightComponent::OnBeginPlay()
	{
		GetRenderer().RegisterSpotLight(*this);
	}

	void SpotLightComponent::OnTrsfChanged()
	{
		data_.pos = GetWorldPos();
		data_.dir = GetForward();
	}
}
