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
    DELETE_CPMV(SpriteComponent);

    explicit SpriteComponent(AActor& owner, int draw_order = 100, int update_order = 100);
    ~SpriteComponent();

    /**
     * Set new texture.
     * @param texture New texture. Null NOT allowed.
     */
    void SetTexture(std::shared_ptr<Texture> texture) noexcept
    {
        texture_ = std::move(texture);
    }

    /**
     * Get current texture.
     * @return Texture. Guaranteed not null.
     */
    [[nodiscard]] auto& GetTexture() const noexcept
    {
        return texture_;
    }

    [[nodiscard]] int GetDrawOrder() const noexcept
    {
        return draw_order_;
    }

protected:
    void OnBeginPlay() override;

private:
    std::shared_ptr<Texture> texture_;
    int draw_order_;
};
}
}
