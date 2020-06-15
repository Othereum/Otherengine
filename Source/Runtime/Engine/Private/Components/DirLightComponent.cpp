#include "Components/DirLightComponent.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"

namespace oeng
{
	DirLightComponent::DirLightComponent(AActor& owner, int update_order)
		:SceneComponent{owner, update_order}, data_{GetForward(), Vec3::one}
	{
	}

	DirLightComponent::~DirLightComponent()
	{
		GetEngine().GetRenderer().UnregisterDirLight(*this);
	}

	void DirLightComponent::Activate() const noexcept
	{
		GetEngine().GetRenderer().RegisterDirLight(*this);
	}
}
