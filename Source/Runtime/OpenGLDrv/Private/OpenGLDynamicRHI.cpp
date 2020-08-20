#include "OpenGLDynamicRHI.hpp"
#include "OpenGLTexture.hpp"

namespace oeng
{
inline namespace opengldrv
{
RHITexture* OpenGLDynamicRHI::CreateTexture(const TextureData& data) const
{
    return new OpenGLTexture{data};
}
}
}
