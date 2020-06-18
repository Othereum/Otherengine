#pragma once
#include "Components/SceneComponent.hpp"
#include "Interfaces/Camera.hpp"

namespace oeng
{
	class OEAPI CameraComponent : public SceneComponent, public ICamera
	{
	public:
		OE_DELETE_CPMV(CameraComponent);
		
		explicit CameraComponent(AActor& owner, int update_order = 100);
		~CameraComponent();

		void SetVFov(Rad vfov) noexcept { data_.vfov = vfov; RecalcProj(); }
		void SetNearFar(Float near, Float far) noexcept { data_.near = near; data_.far = far; RecalcProj(); }

		const Vec3& GetPos() const noexcept override;
		const Data& GetData() const noexcept override { return data_;}
		const Mat4& GetViewProj() const noexcept override;
		void OnScreenSizeChanged(Vec2u16 scr) noexcept override;

	private:
		void OnTrsfChanged() noexcept override;
		void RecalcView() noexcept;
		void RecalcProj() noexcept;
		void OnActivated() override;
		void OnDeactivated() override;
		
		Mat4 view_, proj_;
		Mat4 view_proj_;
		Data data_{10, 10000, 60_deg};
		Vec2 scr_;
	};
}
