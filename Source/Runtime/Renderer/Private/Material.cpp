#include "Material.hpp"
#include "Log.hpp"
#include "Json.hpp"
#include "Renderer.hpp"

namespace oeng
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
				log::Error("'{}': failed to load uniforms: {}", e.what());
			}
		}
	}

	void Material::TryUniforms()
	{	
		for (auto& [name, var] : uniforms_)
		{
			auto set_uniform = [&](auto& val) { return shader_->TryUniform(name, val); };
			std::visit(set_uniform, var);
		}
	}

	void Material::LoadUniforms(const Json& uniforms)
	{
		for (auto& [name, value] : uniforms.items())
		{
			try
			{
				const auto location = shader_->GetUniformLocation(name);
				if (location == Shader::invalid_uniform_) throw std::out_of_range{"could not be found"};
				
				LoadUniform(location, value);
			}
			catch (const std::exception& e)
			{
				log::Error("'{}': invalid uniform '{}': {}", GetPath().Str(), name, e.what());
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

	void Material::LoadUniform(int location, const Json& value)
	{
		auto emplace = [&]<class T>(T&& val) { uniforms_.try_emplace(location, std::forward<T>(val)); };
		
		switch (value.type())
		{
		case JsonType::array:
			switch (value.at(0).type())
			{
			case JsonType::array:
				emplace(LoadMatrix(value));
				break;
				
			case JsonType::boolean:
			case JsonType::number_integer:
			case JsonType::number_unsigned:
				emplace(LoadVector<int32_t>(value));
				break;
				
			case JsonType::number_float:
				emplace(LoadVector<Float>(value));
				break;

			default:
				throw std::domain_error{"Only bool, int, float types are supported for vec element"};
			}
			break;
			
		case JsonType::boolean:
		case JsonType::number_integer:
		case JsonType::number_unsigned:
			uniforms_.try_emplace(location, value.get<int32_t>());
			break;
			
		case JsonType::number_float:
			uniforms_.try_emplace(location, value.get<Float>());
			break;
			
		default:
			throw std::domain_error{"Only bool, int, float, vecn, ivecn, bvecn, matn, matnxn types are supported"};
		}
	}
}
