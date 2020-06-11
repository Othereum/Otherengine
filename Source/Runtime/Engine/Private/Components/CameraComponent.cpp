#include "Components/CameraComponent.hpp"
#include "Engine.hpp"

namespace oeng
{
	CameraComponent::CameraComponent(AActor& owner, int update_order)
		:SceneComponent{owner, update_order},
		view_{MakeLookAt(Vec3::zero, Vec3::forward, Vec3::up)}
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

	void CameraComponent::SetNearFar(Float near, Float far) noexcept
	{
		near_ = near;
		far_ = far;
		RecalcProj();
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

	void CameraComponent::RecalcView() noexcept try
	{
		view_ = MakeLookAt(GetWorldPos(), GetWorldPos() + *GetForward(), Vec3::up);
		view_proj_ = view_ * proj_;
	}
	catch (...) {}

	void CameraComponent::RecalcProj() noexcept
	{
		proj_ = MakePerspective(scr_, near_, far_, vfov_);
		view_proj_ = view_ * proj_;
	}
}
