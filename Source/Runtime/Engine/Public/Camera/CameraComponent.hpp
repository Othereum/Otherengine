#pragma once
#include "Components/SceneComponent.hpp"
#include "Interfaces/Camera.hpp"

namespace oeng
{
	class OEAPI CameraComponent : public SceneComponent, public ICamera
	{
	public:
		explicit CameraComponent(AActor& owner, int update_order = 100);
		~CameraComponent();

		void Activate() noexcept;

		void SetVFov(Rad vfov) noexcept { vfov_ = vfov; RecalcProj(); }
		void SetNearFar(Float near, Float far) noexcept;
		
		[[nodiscard]] Rad GetVFov() const noexcept { return vfov_; }
		[[nodiscard]] Float GetNear() const noexcept { return near_; }
		[[nodiscard]] Float GetFar() const noexcept { return far_; }

		const Vec3& GetPos() const noexcept override;
		const Mat4& GetViewProj() const noexcept override;
		void OnScreenSizeChanged(Vec2u16 scr) noexcept override;

	private:
		void OnTrsfChanged() noexcept override;
		void RecalcView() noexcept;
		void RecalcProj() noexcept;
		
		Mat4 view_, proj_;
		Mat4 view_proj_;
		
		Float near_ = 10, far_ = 10000;
		Rad vfov_ = 60_deg;
		
		Vec2 scr_;
	};
}
