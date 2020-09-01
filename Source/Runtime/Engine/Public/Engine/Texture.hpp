#pragma once

namespace oeng
{
inline namespace rhi
{
class RHITexture;
}

inline namespace engine
{
class ENGINE_API Texture final : public Object
{
public:
    void from_json(const Json& json) override;

    [[nodiscard]] Vec2i Size() const noexcept
    {
        return size_;
    }

private:
    Vec2i size_;
    std::unique_ptr<RHITexture> rhi_;
};
}
}
