#include "Material.hpp"
#include "Log.hpp"
#include "Json.hpp"
#include "Renderer.hpp"

namespace oeng::renderer
{
	Material::Material(Path path, Renderer& renderer)
		:Material{path, renderer, ReadFileAsJson(path)}
	{
	}

	Material::Material(Path path, Renderer& renderer, const Json& json)
		:Asset{path},
		shader_{renderer.GetShader(json.at("shader"))},
		texture_{renderer.GetTexture(json.at("texture"))}
	{
		if (const auto uniforms = json.find("uniforms"); uniforms != json.end())
		{
			try
			{
				LoadUniforms(*uniforms);
			}
			catch (const std::exception& e)
			{
				log::Error(u8"'{}': failed to load uniforms: {}", What(e));
			}
		}
	}

	void Material::TryUniforms()
	{	
		for (auto& uniform : uniforms_)
		{
			auto set_uniform = [&](auto& val) { return shader_->TryUniform(uniform.first, val); };
			if (!std::visit(set_uniform, uniform.second))
			{
				OE_DLOG(1s, log::Level::kErr, u8"{}: Failed to set uniform '{}'", GetPath().Str(), *uniform.first);
			}
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
		case 2: return value.get<Matrix<Float, Row, 2>>();
		case 3: return value.get<Matrix<Float, Row, 3>>();
		case 4: return value.get<Matrix<Float, Row, 4>>();
		default: ThrowLength();
		}
	}

	static Uniform LoadMatrix(const Json& value)
	{
		switch (value.size())
		{
		case 2: return LoadMatrix<2>(value);
		case 3: return LoadMatrix<3>(value);
		case 4: return LoadMatrix<4>(value);
		default: ThrowLength();
		}
	}

	template <class T>
	static Uniform LoadVector(const Json& value)
	{
		switch (value.size())
		{
		case 2: return value.get<Vector<T, 2>>();
		case 3: return value.get<Vector<T, 3>>();
		case 4: return value.get<Vector<T, 4>>();
		default: ThrowLength();
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
		for (auto& [name1, value] : uniforms.items())
		{
			auto& name = AsString8(name1);
			try
			{
				if (shader_->GetUniformLocation(name) == Shader::invalid_uniform_)
					throw std::out_of_range{"could not be found"};
				
				uniforms_.try_emplace(name, LoadUniform(value));
			}
			catch (const std::exception& e)
			{
				log::Error(u8"'{}': invalid uniform '{}': {}", GetPath().Str(), name, What(e));
			}
		}
	}
}
