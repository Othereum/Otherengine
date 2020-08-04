#pragma once
#include "Actors/Actor.hpp"

ENGINE_BEGIN

class DirLightComponent;

class ENGINE_API ADirLight : public AActor
{
public:
	explicit ADirLight(World& world);

	void Activate() const noexcept;
	
	void SetColor(const Vec3& color) const noexcept;
	[[nodiscard]] const Vec3& GetColor() const noexcept;

private:
	DirLightComponent& light_;
};

ENGINE_END
