#include "Engine/Material.hpp"

namespace oeng::engine
{
Material::Material(Path path, Renderer& renderer)
    : Material{path, renderer, ReadFileAsJson(path)}
{
}

[[nodiscard]] static std::shared_ptr<Texture> LoadTexture(Renderer& renderer, const Json& json)
{
    const auto found = json.find("texture");
    return found != json.end()
               ? renderer.GetTexture(found->get<Path>())
               : renderer.GetDefaultTexture();
}

Material::Material(Path path, Renderer& renderer, const Json& json)
    : Asset{path},
      shader_{renderer.GetShader(json.at("shader").get<Path>())},
      texture_{LoadTexture(renderer, json)}
{
    if (const auto uniforms = json.find("uniforms"); uniforms != json.end())
    {
        LoadUniforms(*uniforms);
    }
}

void Material::TryUniforms()
{
    for (auto& uniform : uniforms_)
    {
        auto set_uniform = [&](auto& val)
        {
            shader_->SetUniform(uniform.first, val);
        };
        std::visit(set_uniform, uniform.second);
    }
}

[[noreturn]] static void ThrowLength()
{
    throw std::length_error{"The length must be 2 to 4"};
}

template <size_t Row>
static Uniform LoadMatrix(const Json& value)
{
    switch (value.at(0).size())
    {
    case 2:
        return value.get<Matrix<Float, Row, 2>>();
    case 3:
        return value.get<Matrix<Float, Row, 3>>();
    case 4:
        return value.get<Matrix<Float, Row, 4>>();
    default:
        ThrowLength();
    }
}

static Uniform LoadMatrix(const Json& value)
{
    switch (value.size())
    {
    case 2:
        return LoadMatrix<2>(value);
    case 3:
        return LoadMatrix<3>(value);
    case 4:
        return LoadMatrix<4>(value);
    default:
        ThrowLength();
    }
}

template <class T>
static Uniform LoadVector(const Json& value)
{
    switch (value.size())
    {
    case 2:
        return value.get<Vector<T, 2>>();
    case 3:
        return value.get<Vector<T, 3>>();
    case 4:
        return value.get<Vector<T, 4>>();
    default:
        ThrowLength();
    }
}

static Uniform LoadUniform(const Json& value)
{
    switch (value.type())
    {
    case JsonType::array:
        switch (value.at(0).type())
        {
        case JsonType::array:
            return LoadMatrix(value);

        case JsonType::boolean:
        case JsonType::number_integer:
        case JsonType::number_unsigned:
            return LoadVector<int>(value);

        case JsonType::number_float:
            return LoadVector<Float>(value);

        default:
            throw std::domain_error{"Only bool, int, float types are supported for vec element"};
        }

    case JsonType::boolean:
    case JsonType::number_integer:
    case JsonType::number_unsigned:
        return value.get<int>();

    case JsonType::number_float:
        return value.get<Float>();

    default:
        throw std::domain_error{"Only bool, int, float, vecn, ivecn, bvecn, matn, matnxn types are supported"};
    }
}

void Material::LoadUniforms(const Json& uniforms)
{
    for (auto& [name_narrow, value] : uniforms.items())
    {
        const Name name{AsString8(name_narrow)};
        const auto location = shader_->GetUniformLocation(name);

        if (location == Shader::invalid_uniform)
        {
            OE_LOG(kRenderer, kWarn, u8"'{}': uniform '{}' not found in shader"sv, GetPath().Str(),
                   AsString8(name_narrow));
            continue;
        }

        try
        {
            uniforms_.try_emplace(location, LoadUniform(value));
        }
        catch (const std::exception& e)
        {
            OE_LOG(kRenderer, kErr, u8"'{}': invalid uniform '{}': {}"sv, GetPath().Str(), AsString8(name_narrow),
                   AsString8(e.what()));
        }
    }
}
}
