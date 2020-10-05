#pragma once
#include "SceneComponent.hpp"

namespace oeng
{
inline namespace engine
{
class ENGINE_API PrimitiveComponent : public SceneComponent
{
    CLASS_BODY(PrimitiveComponent)

  public:
    [[nodiscard]] IRenderer& GetRenderer() const noexcept;

    [[nodiscard]] const Mat4& GetWorldTrsfMatrix() const noexcept
    {
        return world_mat_;
    }

  private:
    Mat4 world_mat_ = Mat4::identity;
    bool mat_valid_ = false;
};
} // namespace engine
} // namespace oeng
