#include "Materials/Material.hpp"
#include "DynamicRHI.hpp"
#include "RHIShader.hpp"

namespace oeng
{
inline namespace engine
{
void Material::from_json(const Json& json)
{
    LoadParams(json.at("params"));

    const auto& shaders = json.at("shaders");
    shader_.reset(DynamicRHI::Get().CreateShader(
        ReadFileAsString<char>(shaders.at("vertex").get<std::string>()).c_str(),
        ReadFileAsString<char>(shaders.at("fragment").get<std::string>()).c_str()));
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
}
}
