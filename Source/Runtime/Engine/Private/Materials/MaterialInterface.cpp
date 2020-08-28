#include "Materials/MaterialInterface.hpp"
#include "AssetManager.hpp"

namespace oeng
{
inline namespace engine
{
template <class T, class Fn>
void LoadParam(const Json& json, const std::string& name, std::unordered_map<Name, T>& out, Fn&& fn)
{
    out.clear();
    for (const auto& [name, value] : json.at(name).items())
        out.emplace(AsString8(name), fn(value));
}

template <class T>
void LoadParam(const Json& json, const std::string& name, std::unordered_map<Name, T>& out)
{
    LoadParam(json, name, out, [](const Json& value)
    {
        return value.get<T>();
    });
}

void MaterialInterface::LoadParams(const Json& json)
{
    LoadParam(json, "scalars"s, scalars_);
    LoadParam(json, "vectors"s, vectors_);
    LoadParam(json, "textures"s, textures_, [](const Json& value)
    {
        return AssetManager::Get().Load<Texture>(value.get<Path>());
    });
}
}
}
