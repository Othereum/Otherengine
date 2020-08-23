#include "ShaderParam.hpp"

namespace oeng
{
inline namespace rhi
{
[[noreturn]] static void ThrowLength()
{
    throw std::length_error{"The length must be 2 to 4"};
}

template <size_t Row>
[[nodiscard]] static ShaderParam LoadMatrix(const Json& json)
{
    switch (json.at(0).size())
    {
    case 2:
        return json.get<Matrix<Float, Row, 2>>();
    case 3:
        return json.get<Matrix<Float, Row, 3>>();
    case 4:
        return json.get<Matrix<Float, Row, 4>>();
    default:
        ThrowLength();
    }
}

[[nodiscard]] static ShaderParam LoadMatrix(const Json& json)
{
    switch (json.size())
    {
    case 2:
        return LoadMatrix<2>(json);
    case 3:
        return LoadMatrix<3>(json);
    case 4:
        return LoadMatrix<4>(json);
    default:
        ThrowLength();
    }
}

template <class T>
[[nodiscard]] static ShaderParam LoadVector(const Json& json)
{
    switch (json.size())
    {
    case 2:
        return json.get<Vector<T, 2>>();
    case 3:
        return json.get<Vector<T, 3>>();
    case 4:
        return json.get<Vector<T, 4>>();
    default:
        ThrowLength();
    }
}

[[nodiscard]] static ShaderParam LoadParam(const Json& json)
{
    switch (json.type())
    {
    case JsonType::array:
        switch (json.at(0).type())
        {
        case JsonType::array:
            return LoadMatrix(json);

        case JsonType::boolean:
        case JsonType::number_integer:
        case JsonType::number_unsigned:
            return LoadVector<int>(json);

        case JsonType::number_float:
            return LoadVector<Float>(json);

        default:
            throw std::domain_error{"Only bool, int, float types are supported for vec element"};
        }

    case JsonType::boolean:
    case JsonType::number_integer:
    case JsonType::number_unsigned:
        return json.get<int>();

    case JsonType::number_float:
        return json.get<Float>();

    default:
        throw std::domain_error{"Only bool, int, float, vecn, ivecn, bvecn, matn, matnxn types are supported"};
    }
}

void from_json(const Json& json, ShaderParam& param)
{
    param = LoadParam(json);
}
}
}
