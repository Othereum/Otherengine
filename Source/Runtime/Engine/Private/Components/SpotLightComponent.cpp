#include "Components/SpotLightComponent.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"

namespace oeng
{
	SpotLightComponent::SpotLightComponent(AActor& owner, int update_order)
		:SceneComponent{owner, update_order},
		data_{GetWorldPos(), Vec3::one, GetForward(), Cos(44_deg), 1000}
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
			& !IsNearlyEqual(data_.angle_cos, 1);
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
