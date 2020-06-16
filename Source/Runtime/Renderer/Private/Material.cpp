#include "Material.hpp"
#include "Log.hpp"
#include "Format.hpp"
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

	void Material::LoadUniform(int location, const Json& value)
	{
		switch (value.type())
		{
		case JsonType::array:
			switch (const auto size = value.size())
			{
			case 2:
				uniforms_.try_emplace(location, value.get<Vec2>());
				break;
			case 3:
				uniforms_.try_emplace(location, value.get<Vec3>());
				break;
			case 4:
				uniforms_.try_emplace(location, value.get<Vec4>());
				break;
			default:
				throw std::domain_error{format("The length of the vector must be 2 to 4. (actual: {})", size)};
			}
			break;
		case JsonType::number_integer:
		case JsonType::number_unsigned:
		case JsonType::number_float:
			uniforms_.try_emplace(location, value.get<Float>());
			break;
		default:
			throw std::domain_error{"Only float, vec2, vec3, and vec4 types are supported."};
		}
	}
}
