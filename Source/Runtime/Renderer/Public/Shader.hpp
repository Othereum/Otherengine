#pragma once
#include <compare>
#include "Asset.hpp"
#include "Uniform.hpp"
#include "Templates/HashMap.hpp"

namespace oeng::renderer
{
	class RENDERER_API Shader : public Asset
	{
	public:
		static constexpr int invalid_uniform = -1;

		Shader() = default;
		explicit Shader(Path path);
		
		void Load(Path path)
		{
			Shader{path}.swap(*this);
		}
		
		~Shader();
		
		Shader(Shader&& r) noexcept
			:Asset{std::move(r)}, vert_shader_{r.vert_shader_}, frag_shader_{r.frag_shader_},
			shader_program_{r.shader_program_}, loc_cache_{std::move(r.loc_cache_)},
			uniform_cache_{std::move(r.uniform_cache_)}
		{
			r.vert_shader_ = 0;
			r.frag_shader_ = 0;
			r.shader_program_ = 0;
		}
		
		Shader& operator=(Shader&& r) noexcept
		{
			Shader{std::move(r)}.swap(*this);
			return *this;
		}

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		std::strong_ordering operator<=>(const Shader& r) const noexcept
		{
			return shader_program_ <=> r.shader_program_;
		}

		void Activate() const;
		
		/**
		 * Try to set uniform variable of this shader with given name.
		 * If the name is not valid, it returns false quietly.
		 * Otherwise, error will be logged if occurs.
		 * @param name The name of the uniform variable.
		 * @param value The new value to be set.
		 * @return True if successful.
		 */
		bool TryUniform(Name name, const Uniform& value)
		{
			const auto location = GetUniformLocation(name);
			if (location == invalid_uniform) return false;
			return SetUniform(location, value);
		}

		/**
		 * Set uniform variable of this shader with given name.
		 * An error will be logged if occurs.
		 * @param name The name of the uniform variable. Must be valid.
		 * @param value The new value to be set.
		 * @return True if successful.
		 */
		bool SetUniform(Name name, const Uniform& value)
		{
			return SetUniform(GetUniformLocation(name), value);
		}

		/**
		 * Set uniform variable of this shader with given location.
		 * An error will be logged if occurs.
		 * @param location The location of the uniform variable. Must be valid.
		 * @param value The new value to be set.
		 * @return True if successful.
		 */
		bool SetUniform(int location, const Uniform& value);
		
		/**
		 * Get location of the uniform variable
		 * @param name The name of the uniform
		 * @return Location of the uniform or invalid_uniform if name is invalid
		 */
		[[nodiscard]] int GetUniformLocation(Name name) noexcept;

		void swap(Shader& r) noexcept
		{
			Asset::swap(r);
			using std::swap;
			swap(vert_shader_, r.vert_shader_);
			swap(frag_shader_, r.frag_shader_);
			swap(shader_program_, r.shader_program_);
			swap(loc_cache_, r.loc_cache_);
			swap(uniform_cache_, r.uniform_cache_);
		}

	private:
		friend std::hash<Shader>;

		unsigned vert_shader_ = 0;
		unsigned frag_shader_ = 0;
		unsigned shader_program_ = 0;
		HashMap<Name, int> loc_cache_;
		HashMap<int, Uniform> uniform_cache_;
	};

	inline void swap(Shader& a, Shader& b) noexcept { a.swap(b); }
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
template <>
struct std::hash<oeng::renderer::Shader>
{
	size_t operator()(const oeng::renderer::Shader& key) const noexcept
	{
		return size_t(key.shader_program_);
	}
};
#endif
