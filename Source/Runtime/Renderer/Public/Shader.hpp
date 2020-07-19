#pragma once
#include <compare>
#include "Asset.hpp"
#include "Uniform.hpp"
#include "Templates/HashMap.hpp"

namespace oeng::renderer
{
	class OEAPI Shader : public Asset
	{
	public:
		static constexpr int invalid_uniform_ = -1;

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
		 * Set uniform variable of this shader with given name.
		 * @param name The name of the uniform variable. Must be valid.
		 * @param value The new value to be set.
		 * @throw std::out_of_range If name is invalid
		 */
		void SetUniform(Name name, const Uniform& value)
		{
			SetUniform(GetUniformLocation(name), value);
		}

		/**
		 * Set uniform variable of this shader with given name.
		 * @param location The location of the uniform variable. Must be valid.
		 * @param value The new value to be set.
		 * @throw std::out_of_range If location is invalid
		 */
		void SetUniform(int location, const Uniform& value)
		{
			if (!TryUniform(location, value))
				throw std::out_of_range{"location is invalid"};
		}
		
		/**
		 * Try to set uniform variable of this shader with given name.
		 * @param name The name of the uniform variable. Must be valid.
		 * @param value The new value to be set.
		 * @return true if successful
		 */
		bool TryUniform(Name name, const Uniform& value)
		{
			return TryUniform(GetUniformLocation(name), value);
		}

		/**
		 * Set uniform variable with given location.
		 * @param location The location of the uniform variable. Must be valid.
		 * @param value New value of uniform
		 * @return true if successful
		 */
		bool TryUniform(int location, const Uniform& value);
		
		/**
		 * Get location of the uniform variable
		 * @param name The name of the uniform
		 * @return Location of the uniform or invalid_uniform_ if name is invalid
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

template <>
struct std::hash<oeng::renderer::Shader>
{
	size_t operator()(const oeng::renderer::Shader& key) const noexcept
	{
		return size_t(key.shader_program_);
	}
};
