#pragma once
#include "SceneComponent.hpp"
#include "Interfaces/Light.hpp"

namespace oeng
{
	class OEAPI PointLightComponent : public SceneComponent, public IPointLight
	{
	public:
		OE_DELETE_CPMV(PointLightComponent);
		
		explicit PointLightComponent(AActor& owner, int update_order = 100);
		~PointLightComponent();

		void SetRadius(float radius) noexcept { data_.radius = radius; }
		void SetColor(const Vec3& color) noexcept { data_.color = color; }
		[[nodiscard]] float GetRadius() const noexcept { return data_.radius; }
		[[nodiscard]] const Vec3& GetColor() const noexcept { return data_.color; }
		[[nodiscard]] const Data& GetData() const noexcept override { return data_; }
		[[nodiscard]] bool ShouldAffect() const noexcept override
		{
			return IsEnabled() && data_.radius > kSmallNum && data_.color.LenSqr() > kSmallNum;
		}

	private:
		void OnBeginPlay() override;
		void OnTrsfChanged() override { data_.pos = GetWorldPos(); }
		
		Data data_;
	};
}