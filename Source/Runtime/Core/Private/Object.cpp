#include "Object.hpp"

namespace oeng
{
inline namespace core
{

static std::unordered_map<Name, SharedRef<Object> (*)()> object_creation_map;

SharedRef<Object> NewObject(Name type)
{
    auto ptr = object_creation_map.at(type)();
    return ptr;
}

void RegisterClass(Name type, SharedRef<Object> (*creator)())
{
    auto [it, inserted] = object_creation_map.emplace(type, creator);
    assert(inserted || it->second == creator);
}

} // namespace core
} // namespace oeng
