#include "Object/Factory.hpp"

namespace oeng
{
inline namespace core
{
Factory& Factory::Get()
{
    static Factory factory;
    return factory;
}
}
}
