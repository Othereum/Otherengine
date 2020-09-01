#include "Materials/MaterialInterface.hpp"
#include "AssetManager.hpp"

namespace oeng
{
inline namespace engine
{
template <> bool MaterialInterface::IsValidParam<Float>(Name name) const
{
    return IsScalarParam(name);
}

template <> bool MaterialInterface::IsValidParam<Vec4>(Name name) const
{
    return IsVectorParam(name);
}

template <> bool MaterialInterface::IsValidParam<std::shared_ptr<Texture>>(Name name) const
{
    return IsTextureParam(name);
}

template <class T, class Fn>
void MaterialInterface::LoadParams(const Json& json, std::unordered_map<Name, T>& out, Fn&& fn)
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
void MaterialInterface::LoadParams(const Json& json, std::unordered_map<Name, T>& out)
{
    LoadParams(json, out, [](const Json& value)
    {
        return value.get<T>();
    });
}

void MaterialInterface::LoadParams(const Json& json)
{
    LoadParams(json.at("scalars"s), scalars_);
    LoadParams(json.at("vectors"s), vectors_);
    LoadParams(json.at("textures"s), textures_, [](const Json& value)
    {
        return AssetManager::Get().Load<Texture>(value.get<Path>());
    });
}
}
}
