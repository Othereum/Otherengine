#pragma once
#include "DynamicRHI.hpp"

namespace oeng
{
inline namespace opengldrv
{
class OPENGLDRV_API OpenGLDynamicRHI : public DynamicRHI
{
public:
    [[nodiscard]] RHIResource* CreateTexture(Vec2i size, int channels, const void* pixels) const override;
    [[nodiscard]] RHIResource* CreateMesh(std::span<const Vertex> vertices,
                                          std::span<const Vec3u16> indices) const override;

    [[nodiscard]] Shader* CreateShader(const char* vertex_shader, const char* frag_shader) const override;
};
}
}
