#include "Object.hpp"

namespace oeng
{
inline namespace core
{
static std::unordered_map<Name, std::shared_ptr<Object>(*)()> object_creation_map;

std::shared_ptr<Object> NewObject(Name type)
{
    auto ptr = object_creation_map.at(type)();
    return ptr;
}

void RegisterClass(Name type, std::shared_ptr<Object> (*creator)())
{
    auto [it, inserted] = object_creation_map.emplace(type, creator);
    assert(inserted || it->second == creator);
}
}
}
