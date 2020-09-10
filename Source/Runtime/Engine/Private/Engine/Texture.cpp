#include "Engine/Texture.hpp"
#include "DynamicRHI.hpp"
#include "Engine/AssetManager.hpp"
#include "RHITexture.hpp"
#include <stb_image.h>

namespace oeng
{
inline namespace engine
{

Texture::Texture() = default;
Texture::~Texture() = default;

SharedRef<Texture> Texture::GetDefault()
{
    return AssetManager::Get().Load<Texture>(u8"../Engine/Assets/T_Default.json"sv);
}

void Texture::from_json(const Json& json)
{
    const auto src = json.at("Source").get<std::string>();

    int channels;
    const std::unique_ptr<unsigned char[]> pixels{stbi_load(src.c_str(), &size_[0], &size_[1], &channels, 0)};

    rhi_.reset(DynamicRHI::Get().CreateTexture(size_, channels, pixels.get()));
}

} // namespace engine
} // namespace oeng
