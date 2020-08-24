#include "Materials/MaterialInstance.hpp"

namespace oeng
{
inline namespace engine
{
void MaterialInstance::Serialize(Archive& ar)
{
    const auto json = ar.ReadAllAsJson();

    // TODO: engine::Shader 클래스 정의
    // TODO: AssetManager::Load<Shader>(...)

    params_.clear();
    for (const auto& [name_str, param] : json.items())
    {
        const Name name{AsString8(name_str)};
        if (!shader_->IsValidParam(name))
        {
            OE_LOG(kEngine, kWarn, u8"'{}': parameter '{}' is not valid."sv, ar.GetName(), *name);
            continue;
        }

        try
        {
            params_.try_emplace(name, param.get<ShaderParam>());
        }
        catch (const std::exception& e)
        {
            OE_LOG(kEngine, kErr, u8"'{}': invalid parameter '{}': {}"sv, ar.GetName(), *name, AsString8(e.what()));
        }
    }
}

void MaterialInstance::ApplyDefaultParams()
{
    for (const auto& [name, param] : params_)
    {
        shader_->SetParam(name, param);
    }
}
}
}
