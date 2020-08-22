#include "OpenGLDynamicRHI.hpp"
#include "OpenGLMesh.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLTexture.hpp"

namespace oeng
{
inline namespace opengldrv
{
RHIResource* OpenGLDynamicRHI::CreateTexture(Vec2i size, int channels, const void* pixels) const
{
    return new OpenGLTexture{size, channels, pixels};
}

RHIResource* OpenGLDynamicRHI::CreateMesh(std::span<const Vertex> vertices, std::span<const Vec3u16> indices) const
{
    return new OpenGLMesh{vertices, indices};
}

Shader* OpenGLDynamicRHI::CreateShader(const char* vertex_shader, const char* frag_shader) const
{
    return new OpenGLShader{vertex_shader, frag_shader};
}
}
}
