#include "Components/PointLightComponent.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"

namespace oeng
{
	PointLightComponent::PointLightComponent(AActor& owner, int update_order)
		:SceneComponent{owner, update_order}, data_{GetWorldPos(), Vec3::one, 1000}
	{
	}

	PointLightComponent::~PointLightComponent()
	{
		PointLightComponent::OnDeactivated();
	}

	bool PointLightComponent::ShouldAffect() const noexcept
	{
		return IsActive() && data_.radius > kSmallNum && data_.color.LenSqr() > kSmallNum;
	}

	void PointLightComponent::OnActivated()
	{
		GetRenderer().RegisterPointLight(*this);
	}

	void PointLightComponent::OnDeactivated()
	{
		GetRenderer().UnregisterPointLight(*this);
	}
}
