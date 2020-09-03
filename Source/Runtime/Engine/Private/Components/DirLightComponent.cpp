#include "Components/DirLightComponent.hpp"
#include "Debug.hpp"
#include "Log.hpp"
#include "Renderer.hpp"

namespace oeng
{
inline namespace engine
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
		EXPECT(!IsAutoActivate() || !GetRenderer().IsDirLightRegistered());
	}
}
