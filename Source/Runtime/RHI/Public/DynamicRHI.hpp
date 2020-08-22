#pragma once
#include <span>

namespace oeng
{
inline namespace rhi
{
class RHIResource;
class Shader;
struct Vertex;

class RHI_API DynamicRHI
{
public:
    INTERFACE_BODY(DynamicRHI);
    [[nodiscard]] static DynamicRHI& Get() noexcept;

    [[nodiscard]] virtual RHIResource* CreateTexture(Vec2i size, int channels, const void* pixels) const = 0;
    [[nodiscard]] virtual RHIResource* CreateMesh(std::span<const Vertex> vertices,
                                                  std::span<const Vec3u16> indices) const = 0;

    [[nodiscard]] virtual Shader* CreateShader(const char* vertex_shader, const char* frag_shader) const = 0;
};
}
}
