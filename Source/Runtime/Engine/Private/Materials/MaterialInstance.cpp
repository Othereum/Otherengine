#include "Materials/MaterialInstance.hpp"
#include "Materials/Material.hpp"
#include "Engine/AssetManager.hpp"

namespace oeng
{
inline namespace engine
{
MaterialInstance::MaterialInstance() : root_{Material::GetDefault()}, parent_{root_}
{
}

[[nodiscard]] static SharedRef<Material> FindRoot(SharedRef<IMaterial> parent) noexcept
{
    if (auto mat = Cast<Material>(SharedPtr(parent)))
        return SharedRef(std::move(mat));
    
    return FindRoot(CastChecked<MaterialInstance>(std::move(parent))->GetParent());
}

void MaterialInstance::from_json(const Json& json)
{
    parent_ = AssetManager::Get().Load<IMaterial>(json.at("Parent").get<Path>());
    root_ = FindRoot(parent_);
    LoadParams(json.at("ParameterOverrides"));
}

RHIShader& MaterialInstance::GetRHI() const noexcept
{
    return parent_->GetRHI();
}

bool MaterialInstance::IsScalarParam(Name name) const
{
    return root_->GetScalarParams().contains(name);
}

bool MaterialInstance::IsVectorParam(Name name) const
{
    return root_->GetVectorParams().contains(name);
}

bool MaterialInstance::IsTextureParam(Name name) const
{
    return root_->GetTextureParams().contains(name);
}
} // namespace engine
} // namespace oeng
