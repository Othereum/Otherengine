#include "Components/DirLightComponent.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"

namespace oeng
{
	DirLightComponent::~DirLightComponent()
	{
		GetEngine().GetRenderer().UnregisterDirLight(*this);
	}

	void DirLightComponent::Activate() const noexcept
	{
		GetEngine().GetRenderer().RegisterDirLight(*this);
	}
}
