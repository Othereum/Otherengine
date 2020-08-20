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
    [[nodiscard]] virtual class RHITexture* CreateTexture(const struct TextureData& data) const = 0;
};
}
}
