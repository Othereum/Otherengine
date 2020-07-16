#pragma once
#include "Actors/Actor.hpp"

namespace oeng::engine
{
	class SkyLightComponent;
	
	class OEAPI ASkyLight : public AActor
	{
	public:
		explicit ASkyLight(World& world);

		void Activate() const noexcept;
		
		void SetColor(const Vec3& color) const noexcept;
		[[nodiscard]] const Vec3& GetColor() const noexcept;

	private:
		SkyLightComponent& light_;
	};
}
