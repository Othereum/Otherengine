#include "OpenGLDynamicRHI.hpp"
#include "OpenGLTexture.hpp"

namespace oeng
{
inline namespace opengldrv
{
RHIResource* OpenGLDynamicRHI::CreateTexture(Vec2i size, int channels, const void* pixels) const
{
    return new OpenGLTexture{size, channels, pixels};
}
}
}
