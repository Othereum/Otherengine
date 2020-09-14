#include "Materials/Material.hpp"
#include "Engine/AssetManager.hpp"
#include "Engine/Texture.hpp"
#include "DynamicRHI.hpp"
#include "RHIShader.hpp"

namespace oeng
{
inline namespace engine
{
SharedRef<Material> Material::GetDefault()
{
    return AssetManager::Get().Load<Material>(u8"../Engine/Assets/M_Default.json"sv);
}

Material::Material() = default;
Material::~Material() = default;

void Material::from_json(const Json& json)
{
    LoadParams(json.at("Parameters"));

    const auto& shaders = json.at("Shaders");
    shader_.reset(
        DynamicRHI::Get().CreateShader(ReadFileAsString<char>(shaders.at("Vertex").get<std::string>()).c_str(),
                                       ReadFileAsString<char>(shaders.at("Fragment").get<std::string>()).c_str()));
}

void Material::ApplyParams() const
{
    for (auto& [name, value] : scalars_)
        shader_->ApplyParam(name, value);

    for (auto& [name, value] : vectors_)
        shader_->ApplyParam(name, value);

    for (auto& [name, value] : textures_)
        shader_->ApplyParam(name, value->GetRHI());
}

RHIShader& Material::GetRHI() const noexcept
{
    return *shader_;
}

bool Material::IsScalarParam(Name name) const
{
    return shader_->IsScalarParam(name);
}

bool Material::IsVectorParam(Name name) const
{
    return shader_->IsVectorParam(name);
}

bool Material::IsTextureParam(Name name) const
{
    return shader_->IsTextureParam(name);
}
} // namespace engine
} // namespace oeng
