#include "Components/SkyLightComponent.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"

namespace oeng
{
	SkyLightComponent::~SkyLightComponent()
	{
		GetEngine().GetRenderer().UnregisterSkyLight(*this);
	}

	void SkyLightComponent::Activate() const noexcept
	{
		GetEngine().GetRenderer().RegisterSkyLight(*this);
	}
}
