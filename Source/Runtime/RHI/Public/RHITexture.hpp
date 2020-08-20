#pragma once

namespace oeng
{
inline namespace rhi
{
struct TextureData
{
    Vec2i size;
    int channels = 0;
    std::unique_ptr<const unsigned char[]> pixels;
};

class RHITexture
{
public:
    INTERFACE_BODY(RHITexture);
    virtual void Activate() const noexcept = 0;
};
}
}
