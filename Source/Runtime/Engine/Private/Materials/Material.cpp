#include "Materials/Material.hpp"
#include "DynamicRHI.hpp"

namespace oeng
{
inline namespace engine
{
void Material::Serialize(Archive& ar)
{
    const auto json = ar.ReadAllAsJson();
    LoadParams(json.at("param_defaults"s));

    shader_.reset(DynamicRHI::Get().CreateShader(
        ReadFileAsString<char>(json.at("vertex_shader").get<std::string>()).c_str(),
        ReadFileAsString<char>(json.at("frag_shader").get<std::string>()).c_str()));
}
}
}
