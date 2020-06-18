#pragma once
#include "SceneComponent.hpp"
#include "Interfaces/Light.hpp"

namespace oeng
{
	class OEAPI PointLightComponent : public SceneComponent, public IPointLight
	{
	public:
		explicit PointLightComponent(AActor& owner, int update_order = 100)
			:SceneComponent{owner, update_order}, data_{GetWorldPos(), Vec3::one, 1000}
		{
		}

		void SetRadius(float radius) noexcept { data_.radius = radius; }
		void SetColor(const Vec3& color) noexcept { data_.color = color; }
		[[nodiscard]] float GetRadius() const noexcept { return data_.radius; }
		[[nodiscard]] const Vec3& GetColor() const noexcept { return data_.color; }
		[[nodiscard]] const Data& GetData() const noexcept override { return data_; }

	private:
		void OnTrsfChanged() override
		{
			data_.pos = GetWorldPos();
		}
		
		Data data_;
	};
}