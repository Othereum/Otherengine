#include "Materials/MaterialInterface.hpp"

namespace oeng
{
inline namespace engine
{
void MaterialInterface::LoadParams(const Json& json)
{
    for (const auto& [name, value] : json.at("scalars").items())
    {
        scalars_.emplace(AsString8(name), value.get<Float>());
    }
}
}
}
