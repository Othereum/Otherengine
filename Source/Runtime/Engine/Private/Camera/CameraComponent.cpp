#include "Camera/CameraComponent.hpp"
#include "Renderer.hpp"

namespace oeng
{
	CameraComponent::CameraComponent(AActor& owner, int update_order)
		:SceneComponent{owner, update_order},
		view_{*MakeLookAt(Vec3::zero, UVec3::forward, UVec3::up)}
	{
	}

	CameraComponent::~CameraComponent()
	{
		CameraComponent::OnDeactivated();
	}

	const Vec3& CameraComponent::GetPos() const noexcept
	{
		return GetWorldPos();
	}

	const Mat4& CameraComponent::GetViewProj() const noexcept
	{
		return view_proj_;
	}

	void CameraComponent::OnScreenSizeChanged(Vec2u16 scr) noexcept
	{
		scr_ = Vec2{scr};
		RecalcProj();
	}

	void CameraComponent::OnTrsfChanged() noexcept
	{
		RecalcView();
	}

	void CameraComponent::RecalcView() noexcept
	{
		if (auto view = MakeLookAt(GetWorldPos(), GetForward(), UVec3::up))
		{
			view_ = *view;
			view_proj_ = view_ * proj_;
		}
	}

	void CameraComponent::RecalcProj() noexcept
	{
		proj_ = MakePerspective(scr_, data_.near, data_.far, data_.vfov);
		view_proj_ = view_ * proj_;
	}

	void CameraComponent::OnActivated()
	{
		GetRenderer().RegisterCamera(*this);
	}

	void CameraComponent::OnDeactivated()
	{
		GetRenderer().UnregisterCamera(*this);
	}
}
