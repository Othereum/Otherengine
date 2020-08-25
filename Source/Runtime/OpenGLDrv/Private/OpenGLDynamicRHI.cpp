#include "OpenGLDynamicRHI.hpp"
#include "OpenGLMesh.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLTexture.hpp"

namespace oeng
{
inline namespace opengldrv
{
RHITexture* OpenGLDynamicRHI::CreateTexture(Vec2i size, int channels, const unsigned char* pixels) const
{
    return new OpenGLTexture{size, channels, pixels};
}

RHIMesh* OpenGLDynamicRHI::CreateMesh(std::span<const Vertex> vertices, std::span<const Vec3u16> indices) const
{
    return new OpenGLMesh{vertices, indices};
}

RHIShader* OpenGLDynamicRHI::CreateShader(const char* vertex_shader, const char* frag_shader) const
{
    return new OpenGLShader{vertex_shader, frag_shader};
}
}
}
