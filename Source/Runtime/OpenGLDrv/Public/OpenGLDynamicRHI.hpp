#pragma once
#include "DynamicRHI.hpp"

namespace oeng
{
inline namespace opengldrv
{
class OPENGLDRV_API OpenGLDynamicRHI : public DynamicRHI
{
public:
    [[nodiscard]] RHITexture* CreateTexture(Vec2i size, int channels, const unsigned char* pixels) const override;
    [[nodiscard]] RHIMesh* CreateMesh(std::span<const Vertex> vertices,
                                      std::span<const Vec3u16> indices) const override;

    [[nodiscard]] RHIShader* CreateShader(const char* vertex_shader, const char* frag_shader) const override;
};
}
}
