#include "OpenGLDynamicRHI.hpp"
#include "OpenGLTexture.hpp"

namespace oeng
{
inline namespace opengldrv
{
SharedRef<Texture> OpenGLDynamicRHI::CreateTexture(const fs::path& filepath)
{
    return MakeShared<OpenGLTexture>(filepath);
}
}
}
