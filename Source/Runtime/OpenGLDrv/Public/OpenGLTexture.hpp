#pragma once
#include "RHIResource.hpp"

namespace oeng
{
inline namespace opengldrv
{
class OPENGLDRV_API OpenGLTexture : public RHIResource
{
public:
    DELETE_CPMV(OpenGLTexture);

    OpenGLTexture(Vec2i size, int channels, const void* pixels);
    ~OpenGLTexture();

    void Activate() const noexcept override;

private:
    unsigned id_ = 0;
};
}
}