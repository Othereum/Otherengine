#pragma once
#include "Actor.hpp"

namespace oeng
{
inline namespace engine
{
class DirLightComponent;

class ENGINE_API ADirLight : public AActor
{
CLASS_BODY(ADirLight)

public:
    ADirLight();

    void SetColor(const Vec3& color) const noexcept;
    [[nodiscard]] const Vec3& GetColor() const noexcept;

    [[nodiscard]] DirLightComponent& GetDirLightComp() const noexcept
    {
        return light_;
    }

private:
    DirLightComponent& light_;
};
}
}
