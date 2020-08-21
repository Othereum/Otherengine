#pragma once
#include <span>

namespace oeng
{
inline namespace rhi
{
class RHIResource;

class RHI_API DynamicRHI
{
public:
    INTERFACE_BODY(DynamicRHI);

    [[nodiscard]] static DynamicRHI& Get() noexcept;
    [[nodiscard]] virtual RHIResource* CreateTexture(Vec2i size, int channels, const void* pixels) const = 0;
    [[nodiscard]] virtual RHIResource* CreateMesh(std::span<const struct Vertex> vertices,
                                                  std::span<const Vec3u16> indices) const = 0;
};
}
}
