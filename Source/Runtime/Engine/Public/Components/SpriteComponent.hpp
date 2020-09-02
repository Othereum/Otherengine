#pragma once
#include "SceneComponent.hpp"

struct SDL_Renderer;

namespace oeng
{
inline namespace engine
{
class Texture;

class ENGINE_API SpriteComponent final : public SceneComponent
{
CLASS_BODY(SpriteComponent)

public:
    explicit SpriteComponent(AActor& owner, int draw_order = 100, int update_order = 100);
    ~SpriteComponent();

    [[nodiscard]] bool ShouldDraw() const noexcept
    {
        return IsActive();
    }

    [[nodiscard]] int GetDrawOrder() const noexcept
    {
        return draw_order_;
    }

    [[nodiscard]] const Mat4& GetDrawTrsf() const noexcept
    {
        return GetWorldTrsfMatrix();
    }

    void SetTexture(std::shared_ptr<Texture> texture) noexcept
    {
        texture_ = std::move(texture);
    }

    [[nodiscard]] Texture& GetTexture() const noexcept
    {
        return *texture_;
    }

protected:
    void OnBeginPlay() override;

private:
    std::shared_ptr<Texture> texture_;
    int draw_order_;
};
}
}
