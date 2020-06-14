#pragma once
#include <variant>
#include "Name.hpp"
#include "Path.hpp"
#include "Math.hpp"
#include "Templates/HashMap.hpp"

namespace oeng
{
	class Renderer;
	class Shader;
	class Texture;
	
	class OEAPI Material
	{
	public:
		Material(SharedPtr<Shader> shader, SharedPtr<Texture> texture) noexcept
			:shader_{std::move(shader)}, texture_{std::move(texture)}
		{
		}
		
		/**
		 * \brief Load material from file
		 * \param path File path
		 * \param renderer Renderer
		 * \throw std::runtime_error If failed to open or parse
		 */
		Material(Path path, Renderer& renderer);
		void Load(Path path, Renderer& renderer)
		{
			Material{path, renderer}.swap(*this);
		}
		
		void SetUniforms();
		
		[[nodiscard]] Shader& GetShader() const noexcept { return *shader_; }
		[[nodiscard]] Texture& GetTexture() const noexcept { return *texture_; }

		void swap(Material& r) noexcept
		{
			using std::swap;
			swap(path_, r.path_);
			swap(shader_, r.shader_);
			swap(texture_, r.texture_);
			swap(uniforms_, r.uniforms_);
		}

	private:
		void LoadUniforms(const Json& uniforms);
		void LoadUniform(const std::string& name, const Json& value);

		Path path_;
		SharedPtr<Shader> shader_;
		SharedPtr<Texture> texture_;
		HashMap<Name, std::variant<Float, Vec2, Vec3, Vec4>> uniforms_;
	};

	inline void swap(Material& a, Material& b) noexcept
	{
		a.swap(b);
	}
}