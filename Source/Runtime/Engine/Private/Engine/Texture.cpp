#include "Engine/Texture.hpp"
#include "DynamicRHI.hpp"
#include <stb_image.h>

namespace oeng
{
inline namespace engine
{
Archive& operator<<(Archive& ar, Texture& texture)
{
    assert(ar.IsLoading());

    const auto size = ar.Size();
    const auto pixels = std::make_unique<unsigned char[]>(size);
    ar.Serialize(pixels.get(), size);

    texture.data_.pixels.reset(stbi_load_from_memory(pixels.get(), size, &texture.data_.size[0], &texture.data_.size[1],
                                                     &texture.data_.channels, 0));
    texture.rhi_.reset(DynamicRHI::Get().CreateTexture(texture.data_));

    return ar;
}
}
}
