#include "Engine/Texture.hpp"
#include "DynamicRHI.hpp"
#include <stb_image.h>

namespace oeng
{
inline namespace engine
{
void Texture::Serialize(Archive& ar)
{
    assert(ar.IsLoading());

    const auto len = ar.Size();
    const std::unique_ptr<unsigned char[]> raw{new unsigned char[len]};
    ar.Serialize(raw.get(), len);

    int channels;
    const std::unique_ptr<unsigned char[]> pixels{
        stbi_load_from_memory(raw.get(), len, &size_[0], &size_[1], &channels, 0)};
    rhi_.reset(DynamicRHI::Get().CreateTexture(size_, channels, pixels.get()));
}
}
}
