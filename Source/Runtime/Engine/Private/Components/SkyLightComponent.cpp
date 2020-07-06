#include "Components/SkyLightComponent.hpp"

#include "Debug.hpp"
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
		EXPECT(!IsAutoActivate() || !GetRenderer().IsSkyLightRegistered());
	}
}
