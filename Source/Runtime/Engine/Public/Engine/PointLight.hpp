#pragma once
#include "GameFramework/Actor.hpp"

namespace oeng
{
inline namespace engine
{
class PointLightComponent;

class ENGINE_API APointLight : public AActor
{
CLASS_BODY(APointLight)

public:
    APointLight();

    void SetRadius(Float radius) const noexcept;
    void SetColor(const Vec3& color) const noexcept;

    [[nodiscard]] Float GetRadius() const noexcept;
    [[nodiscard]] const Vec3& GetColor() const noexcept;

    [[nodiscard]] PointLightComponent& GetPointLightComp() const noexcept
    {
        return light_;
    }

private:
    PointLightComponent& light_;
};
}
}
