#include "Material.hpp"
#include "Format.hpp"
#include "Json.hpp"
#include "Renderer.hpp"
#include "Log.hpp"

namespace oeng
{
	Material::Material(Path path, Renderer& renderer)
		:path_{path}
	{
		const auto json = ReadFileAsJson(path);

		shader_ = renderer.GetShader(json.at("shader"));
		texture_ = renderer.GetTexture(json.at("texture"));

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

	void Material::LoadUniforms(const Json& uniforms)
	{
		for (auto&& [name, value] : uniforms.items())
		{
			try
			{
				LoadUniform(name, value);
			}
			catch (const std::exception& e)
			{
				log::Error("'{}': invalid uniform '{}': {}", path_->string(), name, e.what());
			}
		}
	}

	void Material::LoadUniform(const std::string& name, const Json& value) noexcept
	{
		switch (value.type())
		{
		case JsonType::array:
			switch (const auto size = value.size())
			{
			case 2:
				uniforms_.try_emplace(name, value.get<Vec2>());
				break;
			case 3:
				uniforms_.try_emplace(name, value.get<Vec3>());
				break;
			case 4:
				uniforms_.try_emplace(name, value.get<Vec4>());
				break;
			default:
				throw std::domain_error{format("The length of the vector must be 2 to 4. (actual: {})", size)};
			}
			break;
		case JsonType::number_integer:
		case JsonType::number_unsigned:
		case JsonType::number_float:
			uniforms_.try_emplace(name, value.get<Float>());
			break;
		default:
			throw std::domain_error{"Only float, vec2, vec3, and vec4 types are supported."};
		}
	}
}
