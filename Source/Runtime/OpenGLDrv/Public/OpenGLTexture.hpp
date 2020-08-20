#pragma once
#include "RHITexture.hpp"

namespace oeng
{
inline namespace opengldrv
{
class OPENGLDRV_API OpenGLTexture : public RHITexture
{
public:
    DELETE_CPMV(OpenGLTexture);

    explicit OpenGLTexture(const TextureData& data);
    ~OpenGLTexture();

    void Activate() const noexcept override;

private:
    unsigned id_;
};
}
}