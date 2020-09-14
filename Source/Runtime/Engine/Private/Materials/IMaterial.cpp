#include "Materials/IMaterial.hpp"
#include "Engine/AssetManager.hpp"
#include "Engine/Texture.hpp"

namespace logcat
{
const LogCategory kMaterial{u8"Material"sv};
}

namespace oeng
{
inline namespace engine
{
IMaterial::~IMaterial() = default;

ScalarParam IMaterial::GetScalarParam(Name name) const
{
    return scalars_.at(name);
}

VectorParam IMaterial::GetVectorParam(Name name) const
{
    return vectors_.at(name);
}

SharedRef<Texture> IMaterial::GetTextureParam(Name name) const
{
    return textures_.at(name);
}

template <> bool IMaterial::IsValidParam<Float>(Name name) const
{
    return IsScalarParam(name);
}

template <> bool IMaterial::IsValidParam<Vec4>(Name name) const
{
    return IsVectorParam(name);
}

template <> bool IMaterial::IsValidParam<SharedRef<Texture>>(Name name) const
{
    return IsTextureParam(name);
}

template <class T, class Fn> void IMaterial::LoadParams(const Json& json, std::unordered_map<Name, T>& out, Fn&& fn)
{
    out.clear();
    for (const auto& [name_str, value] : json.items())
    {
        const Name name = AsString8(name_str);
        if (!IsValidParam<T>(name))
        {
            OE_LOG(kMaterial, kWarn, u8"'{}': parameter '{}' does not exist."sv, GetPath().Str(), *name);
            continue;
        }

        try
        {
            out.emplace(name, fn(value));
        }
        catch (const std::exception& e)
        {
            OE_LOG(kMaterial, kErr, u8"'{}': invalid parameter '{}': {}"sv, GetPath().Str(), *name,
                   AsString8(e.what()));
        }
    }
}

template <class T> void IMaterial::LoadParams(const Json& json, std::unordered_map<Name, T>& out)
{
    LoadParams(json, out, [](const Json& value) { return value.get<T>(); });
}

SharedRef<IMaterial> IMaterial::GetDefault()
{
    return AssetManager::Get().Load<IMaterial>(u8"../Engine/Assets/M_Default.json"sv);
}

void IMaterial::LoadParams(const Json& json)
{
    LoadParams(json.at("Scalars"s), scalars_);
    LoadParams(json.at("Vectors"s), vectors_);
    LoadParams(json.at("Textures"s), textures_,
               [](const Json& value) { return AssetManager::Get().Load<Texture>(value.get<Path>()); });
}
} // namespace engine
} // namespace oeng
