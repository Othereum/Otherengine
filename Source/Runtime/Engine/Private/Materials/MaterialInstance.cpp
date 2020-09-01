#include "Materials/MaterialInstance.hpp"
#include "AssetManager.hpp"
#include "Materials/Material.hpp"

namespace oeng
{
inline namespace engine
{
void MaterialInstance::from_json(const Json& json)
{
    LoadParams(json.at("param_overrides"));
    parent_ = AssetManager::Get().Load<Material>(json.at("parent").get<Path>());
}

RHIShader& MaterialInstance::GetRHIShader() const noexcept
{
    return parent_->GetRHIShader();
}

bool MaterialInstance::IsScalarParam(Name name) const
{
    return parent_->GetScalarParams().contains(name);
}

bool MaterialInstance::IsVectorParam(Name name) const
{
    return parent_->GetVectorParams().contains(name);
}

bool MaterialInstance::IsTextureParam(Name name) const
{
    return parent_->GetTextureParams().contains(name);
}
}
}
