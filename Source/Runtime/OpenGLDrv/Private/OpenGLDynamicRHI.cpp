#include "OpenGLDynamicRHI.hpp"
#include "OpenGLMesh.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLTexture.hpp"
#include <SDL2/SDL.h>

namespace logcat
{
const LogCategory kOpenGL{u8"OpenGL"sv};
}

namespace oeng
{
inline namespace opengldrv
{
static void SetGlAttribute(SDL_GLattr attr, int value)
{
    if (0 != SDL_GL_SetAttribute(attr, value))
        OE_LOG(kOpenGL, kErr, u8"Failed to set attribute {} to {}: {}"sv, attr, value, AsString8(SDL_GetError()));
}

OpenGLDynamicRHI::OpenGLDynamicRHI()
{
    SetGlAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SetGlAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OE_OPENGL_VERSION_MAJOR);
    SetGlAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OE_OPENGL_VERSION_MINOR);
    SetGlAttribute(SDL_GL_RED_SIZE, 8);
    SetGlAttribute(SDL_GL_GREEN_SIZE, 8);
    SetGlAttribute(SDL_GL_BLUE_SIZE, 8);
    SetGlAttribute(SDL_GL_ALPHA_SIZE, 8);
    SetGlAttribute(SDL_GL_DEPTH_SIZE, 24);
    SetGlAttribute(SDL_GL_DOUBLEBUFFER, true);
    SetGlAttribute(SDL_GL_ACCELERATED_VISUAL, true);
}

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
} // namespace opengldrv
} // namespace oeng
