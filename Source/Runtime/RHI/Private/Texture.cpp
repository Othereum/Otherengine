#include "Texture.hpp"
#include <stb_image.h>

namespace oeng
{
inline namespace rhi
{
Texture::Texture(const fs::path& filepath)
    : data_{stbi_load(filepath.string().c_str(), &size_[0], &size_[1], &channels_, 0)}
{
    if (!data_)
        throw std::runtime_error{stbi_failure_reason()};
}
}
}
