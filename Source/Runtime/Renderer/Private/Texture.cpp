#include "Texture.hpp"
#include <GL/glew.h>
#include <SOIL2/SOIL2.h>

namespace oeng::renderer
{
Texture::Texture(Path path)
    : Asset{path}
{
    Vector<int, 2> size;
    auto num_channels = 0;

    const std::unique_ptr<const unsigned char[]> image{
        SOIL_load_image(path->string().c_str(), &size[0], &size[1], &num_channels, SOIL_LOAD_AUTO)
    };

    if (!image)
        throw std::runtime_error{SOIL_last_result()};

    constexpr auto max_size = std::numeric_limits<uint16_t>::max();
    if (size[0] > max_size || size[1] > max_size)
        throw std::runtime_error{fmt::format("Too big ({0}x{1}). Max size is {2}x{2}"sv, size[0], size[1], max_size)};

    int img_format;
    int alignment;
    switch (num_channels)
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
                        num_channels)};
    }

    size_ = Vec2u16{size};
    glGenTextures(1, &id_);
    Activate();

    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glTexImage2D(GL_TEXTURE_2D, 0, img_format, size[0], size[1], 0, img_format, GL_UNSIGNED_BYTE, image.get());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture()
{
    // glDelete silently ignores 0
    glDeleteTextures(1, &id_);
}

void Texture::Activate() const
{
    glBindTexture(GL_TEXTURE_2D, id_);
}
}
