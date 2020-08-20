#include "OpenGLTexture.hpp"
#include <GL/glew.h>

namespace oeng
{
inline namespace opengldrv
{
OpenGLTexture::OpenGLTexture(const fs::path& filepath)
    : Texture{filepath}
{
    int img_format;
    int alignment;
    switch (channels())
    {
    case 3:
        img_format = GL_RGB;
        alignment = 1;
        break;

    case 4:
        img_format = GL_RGBA;
        alignment = 4;
        break;

    default:
        throw std::runtime_error{
            fmt::format("Invalid format: It has {} channels, but only RGB and RGBA formats are supported."sv,
                        channels())};
    }

    glGenTextures(1, &id_);
    OpenGLTexture::Activate();

    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glTexImage2D(GL_TEXTURE_2D, 0, img_format, size()[0], size()[1], 0, img_format, GL_UNSIGNED_BYTE, data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

OpenGLTexture::~OpenGLTexture()
{
    // glDelete silently ignores 0
    glDeleteTextures(1, &id_);
}

void OpenGLTexture::Activate() const noexcept
{
    glBindTexture(GL_TEXTURE_2D, id_);
}
}
}
