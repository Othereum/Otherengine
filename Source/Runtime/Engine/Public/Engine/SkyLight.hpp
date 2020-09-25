#pragma once
#include "GameFramework/Actor.hpp"

namespace oeng
{
inline namespace engine
{
class SkyLightComponent;

class ENGINE_API ASkyLight : public AActor
{
    CLASS_BODY(ASkyLight)

  public:
    ASkyLight();

    void SetColor(const Vec3& color) const noexcept;
    [[nodiscard]] const Vec3& GetColor() const noexcept;

    [[nodiscard]] SkyLightComponent& GetSkyLightComp() const noexcept
    {
        return light_;
    }

  private:
    SkyLightComponent& light_;
};
} // namespace engine
} // namespace oeng
