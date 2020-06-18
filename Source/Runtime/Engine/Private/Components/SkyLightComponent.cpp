#include "Components/SkyLightComponent.hpp"
#include "Log.hpp"
#include "Renderer.hpp"

namespace oeng
{
	SkyLightComponent::~SkyLightComponent()
	{
		SkyLightComponent::OnDeactivated();
	}

	void SkyLightComponent::OnActivated()
	{
		GetRenderer().RegisterSkyLight(*this);
	}

	void SkyLightComponent::OnDeactivated()
	{
		GetRenderer().UnregisterSkyLight(*this);
	}

	void SkyLightComponent::OnBeginPlay()
	{
		if (IsAutoActivate() && GetRenderer().IsSkyLightRegistered())
		{
			log::Warn("Only one sky light can be activated at the same time.");
		}
	}
}
