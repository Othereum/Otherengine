#pragma once

namespace oeng
{
inline namespace rhi
{
class RHI_API DynamicRHI
{
    INTERFACE_BODY(DynamicRHI);

    [[nodiscard]] static DynamicRHI& Get() noexcept;

    [[nodiscard]] virtual SharedRef<class Texture> CreateTexture(const fs::path& filepath) = 0;
};
}
}
