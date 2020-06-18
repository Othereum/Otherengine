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
		GetEngine().GetRenderer().UnregisterPointLight(*this);
	}

	void PointLightComponent::OnBeginPlay()
	{
		GetEngine().GetRenderer().RegisterPointLight(*this);
	}
}
