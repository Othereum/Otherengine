#include "Components/SkyLightComponent.hpp"
#include "Debug.hpp"
#include "Renderer.hpp"

ENGINE_BEGIN

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

ENGINE_END
