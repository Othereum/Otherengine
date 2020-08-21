#pragma once

namespace oeng
{
inline namespace rhi
{
class RHI_API DynamicRHI
{
public:
    INTERFACE_BODY(DynamicRHI);

    [[nodiscard]] static DynamicRHI& Get() noexcept;
    [[nodiscard]] virtual class RHIResource* CreateTexture(Vec2i size, int channels, const void* pixels) const = 0;
};
}
}
