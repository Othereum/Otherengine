#include "ShaderParam.hpp"

namespace oeng
{
inline namespace rhi
{
[[nodiscard]] static ScalarParam LoadScalar(const Json& json)
{
    switch (json.type())
    {
    case JsonType::number_float:
        return json.get<Float>();

    case JsonType::number_integer:
    case JsonType::number_unsigned:
        return json.get<int32_t>();

    default:
        throw std::domain_error("Only float/int types are supported for scalar parameters");
    }
}

template <size_t L>[[nodiscard]] static VectorParam LoadVector(const Json& json)
{
    switch (json.at(0).type())
    {
    case JsonType::number_float:
        return json.get<Vector<Float, L>>();

    case JsonType::number_integer:
    case JsonType::number_unsigned:
        return json.get<Vector<int32_t, L>>();

    default:
        throw std::domain_error("Only float/int types are supported for elements in vector parameters");
    }
}

[[nodiscard]] static VectorParam LoadVector(const Json& json)
{
    if (json.type() != JsonType::array)
        throw std::domain_error("Array expected for vector parameter");

    switch (json.size())
    {
    case 2:
        return LoadVector<2>(json);
    case 3:
        return LoadVector<3>(json);
    case 4:
        return LoadVector<4>(json);
    default:
        throw std::length_error{"The length must be 2 to 4"};
    }
}
} // namespace rhi
} // namespace oeng

namespace std
{
void from_json(const oeng::Json& json, oeng::ScalarParam& param)
{
    param = oeng::LoadScalar(json);
}

void from_json(const oeng::Json& json, oeng::VectorParam& param)
{
    param = oeng::LoadVector(json);
}
} // namespace std
