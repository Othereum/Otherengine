#pragma once
#include "SceneComponent.hpp"
#include "Interfaces/Light.hpp"

namespace oeng
{
	class OEAPI SpotLightComponent : public SceneComponent, public ISpotLight
	{
	public:
		DELETE_CPMV(SpotLightComponent);
		
		explicit SpotLightComponent(AActor& owner, int update_order = 100);
		~SpotLightComponent();

		void SetRadius(float radius) noexcept { data_.radius = radius; }
		void SetColor(const Vec3& color) noexcept { data_.color = color; }
		void SetAngle(Data::Cone angle) noexcept { data_.angle_cos = angle; }
		[[nodiscard]] const Data& GetData() const noexcept override { return data_; }
		[[nodiscard]] bool ShouldAffect() const noexcept override;

	private:
		void OnBeginPlay() override;
		void OnTrsfChanged() override;
		
		Data data_;
	};
}