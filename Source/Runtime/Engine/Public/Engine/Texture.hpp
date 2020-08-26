#pragma once

namespace oeng
{
inline namespace rhi
{
class RHITexture;
}
}

namespace oeng
{
inline namespace engine
{
class ENGINE_API Texture : public ISerializable
{
public:
    void Serialize(Archive& ar) override;

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
