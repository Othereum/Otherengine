#pragma once
#include "RHITexture.hpp"

namespace oeng
{
inline namespace engine
{
class ENGINE_API Texture
{
public:
    friend ENGINE_API Archive& operator<<(Archive& ar, Texture& texture);

    [[nodiscard]] const TextureData& Data() const noexcept
    {
        return data_;
    }

private:
    TextureData data_;
    std::unique_ptr<RHITexture> rhi_;
};
}
}
