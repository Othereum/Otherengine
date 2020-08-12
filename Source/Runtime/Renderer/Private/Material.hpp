#pragma once
#include "Uniform.hpp"

namespace oeng::renderer
{
	class Shader;
	class Texture;
	class Renderer;
	
	class RENDERER_API Material : public Asset
	{
	public:
		Material(SharedRef<Shader> shader, SharedRef<Texture> texture) noexcept
			:shader_{std::move(shader)}, texture_{std::move(texture)}
		{
		}
		
		/**
		 * Load material from file
		 * @param path File path
		 * @param renderer Renderer
		 * @throw std::runtime_error If failed to open or parse
		 */
		Material(Path path, Renderer& renderer);
		void Load(Path path, Renderer& renderer)
		{
			Material{path, renderer}.swap(*this);
		}
		
		void TryUniforms();
		
		[[nodiscard]] Shader& GetShader() const noexcept { return *shader_; }
		[[nodiscard]] Texture& GetTexture() const noexcept { return *texture_; }

		void swap(Material& r) noexcept
		{
			Asset::swap(r);
			using std::swap;
			swap(shader_, r.shader_);
			swap(texture_, r.texture_);
			swap(uniforms_, r.uniforms_);
		}

	private:
		Material(Path path, Renderer& renderer, const Json& json);
		void LoadUniforms(const Json& uniforms);

		SharedRef<Shader> shader_;
		SharedRef<Texture> texture_;
		std::unordered_map<int, Uniform> uniforms_;
	};

	inline void swap(Material& a, Material& b) noexcept
	{
		a.swap(b);
	}
}
