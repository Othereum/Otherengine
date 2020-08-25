#include "Materials/Material.hpp"
#include "RHIShader.hpp"

namespace oeng
{
inline namespace engine
{
void Material::Serialize(Archive& ar)
{
    const auto json = ar.ReadAllAsJson();

}
}
}
