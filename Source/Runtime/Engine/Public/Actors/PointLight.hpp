#pragma once
#include "Actor.hpp"

namespace oeng
{
	class PointLightComponent;
	
	class OEAPI APointLight : public AActor
	{
	public:
		explicit APointLight(World& world);

		void SetRadius(float radius) const noexcept;
		void SetColor(const Vec3& color) const noexcept;
		[[nodiscard]] float GetRadius() const noexcept;
		[[nodiscard]] const Vec3& GetColor() const noexcept;

	private:
		PointLightComponent& light_;
	};
}
