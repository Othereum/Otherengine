#pragma once
#include <variant>
#include "Name.hpp"
#include "Path.hpp"
#include "Templates/HashMap.hpp"

namespace oeng
{
	class Renderer;
	class Shader;
	class Texture;
	
	class OEAPI Material
	{
	public:
		Material() noexcept = default;
		Material(Path path, Renderer& renderer);
		
		void SetUniforms();
		
		[[nodiscard]] Shader& GetShader() const noexcept { return *shader_; }
		[[nodiscard]] Texture& GetTexture() const noexcept { return *texture_; }

	private:
		void LoadUniforms(const Json& uniforms);
		void LoadUniform(const std::string& name, const Json& value);

		Path path_;
		SharedPtr<Shader> shader_;
		SharedPtr<Texture> texture_;
		HashMap<Name, std::variant<Float, Vec2, Vec3, Vec4>> uniforms_;
	};
}