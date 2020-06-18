#include "Components/DirLightComponent.hpp"
#include "Log.hpp"
#include "Renderer.hpp"

namespace oeng
{
	DirLightComponent::DirLightComponent(AActor& owner, int update_order)
		:SceneComponent{owner, update_order}, data_{GetForward(), Vec3::one}
	{
	}

	DirLightComponent::~DirLightComponent()
	{
		DirLightComponent::OnDeactivated();
	}

	void DirLightComponent::OnActivated()
	{
		GetRenderer().RegisterDirLight(*this);
	}

	void DirLightComponent::OnDeactivated()
	{
		GetRenderer().UnregisterDirLight(*this);
	}

	void DirLightComponent::OnBeginPlay()
	{
		if (IsAutoActivate() && GetRenderer().IsDirLightRegistered())
		{
			log::Warn("Only one directional light can be activated at the same time.");
		}
	}
}
