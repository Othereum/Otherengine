#pragma once
#include "Texture.hpp"

namespace oeng
{
inline namespace opengldrv
{
class OPENGLDRV_API OpenGLTexture : public Texture
{
public:
    DELETE_CPMV(OpenGLTexture);

    explicit OpenGLTexture(const fs::path& filepath);
    ~OpenGLTexture();

    void Activate() const noexcept override;

private:
    unsigned id_;
};
}
}