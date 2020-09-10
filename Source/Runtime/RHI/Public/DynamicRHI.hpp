#pragma once
#include <span>

namespace oeng
{
inline namespace rhi
{
class RHITexture;
class RHIMesh;
class RHIShader;
struct Vertex;

class RHI_API DynamicRHI
{
public:
    virtual ~DynamicRHI() = default;
    [[nodiscard]] static DynamicRHI& Get() noexcept;

    [[nodiscard]] virtual RHITexture* CreateTexture(Vec2i size, int channels, const unsigned char* pixels) const = 0;
    [[nodiscard]] virtual RHIMesh* CreateMesh(std::span<const Vertex> vertices,
                                              std::span<const Vec3u16> indices) const = 0;

    [[nodiscard]] virtual RHIShader* CreateShader(const char* vertex_shader, const char* frag_shader) const = 0;
};
}
}
