#include "Components/CameraComponent.hpp"
#include "Engine.hpp"

namespace oeng
{
	CameraComponent::CameraComponent(AActor& owner, int update_order)
		:SceneComponent{owner, update_order}
	{
	}

	CameraComponent::~CameraComponent()
	{
		GetEngine().GetRenderer().UnregisterCamera(*this);
	}

	void CameraComponent::Activate() noexcept
	{
		GetEngine().GetRenderer().RegisterCamera(*this);
	}

	void CameraComponent::RecalcView() noexcept
	{
		view_ = MakeLookAt(GetWorldPos(), GetForward(), Vec3::up);
		view_proj_ = view_ * proj_;
	}

	void CameraComponent::RecalcProj() noexcept
	{
		proj_ = MakePerspective(scr_, near_, far_, hfov_);
		view_proj_ = view_ * proj_;
	}
}
