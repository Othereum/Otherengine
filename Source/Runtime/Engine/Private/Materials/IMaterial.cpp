#include "Materials/IMaterial.hpp"
#include "AssetManager.hpp"

namespace oeng
{
inline namespace engine
{
template <> bool IMaterial::IsValidParam<Float>(Name name) const
{
    return IsScalarParam(name);
}

template <> bool IMaterial::IsValidParam<Vec4>(Name name) const
{
    return IsVectorParam(name);
}

template <> bool IMaterial::IsValidParam<std::shared_ptr<Texture>>(Name name) const
{
    return IsTextureParam(name);
}

template <class T, class Fn>
void IMaterial::LoadParams(const Json& json, std::unordered_map<Name, T>& out, Fn&& fn)
{
    out.clear();
    for (const auto& [name_str, value] : json.items())
    {
        const Name name = AsString8(name_str);
        if (!IsValidParam<T>(name))
        {
            OE_LOG(kEngine, kWarn, u8"'{}': parameter '{}' does not exist."sv, GetPath().Str(), *name);
            continue;
        }

        try
        {
            out.emplace(name, fn(value));
        }
        catch (const std::exception& e)
        {
            OE_LOG(kEngine, kErr, u8"'{}': invalid parameter '{}': {}"sv, GetPath().Str(), *name, AsString8(e.what()));
        }
    }
}

template <class T>
void IMaterial::LoadParams(const Json& json, std::unordered_map<Name, T>& out)
{
    LoadParams(json, out, [](const Json& value)
    {
        return value.get<T>();
    });
}

std::shared_ptr<IMaterial> IMaterial::GetDefault()
{
    return AssetManager::Get().Load<IMaterial>(u8"../Engine/Assets/M_Default.json"sv);
}

void IMaterial::LoadParams(const Json& json)
{
    LoadParams(json.at("Scalars"s), scalars_);
    LoadParams(json.at("Vectors"s), vectors_);
    LoadParams(json.at("Textures"s), textures_, [](const Json& value)
    {
        return AssetManager::Get().Load<Texture>(value.get<Path>());
    });
}
}
}
