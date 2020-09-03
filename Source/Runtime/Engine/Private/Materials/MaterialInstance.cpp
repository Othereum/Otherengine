#include "Materials/MaterialInstance.hpp"
#include "AssetManager.hpp"
#include "Materials/Material.hpp"

namespace oeng
{
inline namespace engine
{
void MaterialInstance::from_json(const Json& json)
{
    LoadParams(json.at("ParameterOverrides"));
    parent_ = AssetManager::Get().Load<Material>(json.at("Parent").get<Path>());
}

RHIShader& MaterialInstance::GetRHI() const noexcept
{
    return parent_->GetRHI();
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
