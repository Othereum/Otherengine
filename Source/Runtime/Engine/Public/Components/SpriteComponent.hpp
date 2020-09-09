#pragma once
#include "SceneComponent.hpp"

namespace oeng
{
inline namespace engine
{
class Texture;

class ENGINE_API SpriteComponent : public SceneComponent
{
CLASS_BODY(SpriteComponent)

public:
    SpriteComponent();

    /**
     * Set new texture.
     * @param texture New texture. Null NOT allowed.
     */
    void SetTexture(SharedRef<Texture> texture) noexcept
    {
        texture_ = std::move(texture);
    }

    /**
     * Get current texture.
     * @return Texture. Guaranteed not null.
     */
    [[nodiscard]] const SharedRef<Texture>& GetTexture() const noexcept
    {
        return texture_;
    }

    void SetDrawOrder(int draw_order);

    [[nodiscard]] int GetDrawOrder() const noexcept
    {
        return draw_order_;
    }

protected:
    void OnBeginPlay() override;
    void OnEndPlay() override;

private:
    SharedRef<Texture> texture_;
    int draw_order_ = 100;
};
}
}
