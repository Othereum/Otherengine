#pragma once
#include "RHITexture.hpp"

namespace oeng
{
inline namespace opengldrv
{
class OPENGLDRV_API OpenGLTexture final : public RHITexture
{
public:
    DELETE_CPMV(OpenGLTexture);

    OpenGLTexture(Vec2i size, int channels, const unsigned char* pixels);
    ~OpenGLTexture();

    void Activate() const noexcept override;

private:
    unsigned id_ = 0;
};
}
}