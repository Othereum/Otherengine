#pragma once
#include "Path.hpp"
#include "Name.hpp"
#include "Templates/HashMap.hpp"

namespace oeng
{
	class OEAPI Shader
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
			:path_{r.path_}, vert_shader_{r.vert_shader_}, frag_shader_{r.frag_shader_},
			shader_program_{r.shader_program_}, uniform_{std::move(r.uniform_)}
		{
			r.path_ = {};
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
		 * \brief Set uniform variable of this shader with given name.
		 * \param name The name of the uniform variable. Must be valid.
		 * \param value The new value to be set.
		 * \throw std::out_of_range If name is invalid
		 */
		template <class T>
		void SetUniform(Name name, const T& value)
		{
			SetUniform(GetUniformLocation(name), value);
		}

		/**
		 * \brief Set uniform variable of this shader with given name.
		 * \param location The location of the uniform variable. Must be valid.
		 * \param value The new value to be set.
		 * \throw std::out_of_range If name is invalid
		 */
		template <class T>
		void SetUniform(int location, const T& value)
		{
			if (!TryUniform(location, value))
				throw std::out_of_range{"location is invalid"};
		}
		
		/**
		 * \brief Try to set uniform variable of this shader with given name.
		 * \param name The name of the uniform variable. Must be valid.
		 * \param value The new value to be set.
		 * \return true if successful
		 */
		template <class T>
		bool TryUniform(Name name, const T& value) noexcept
		{
			return TryUniform(GetUniformLocation(name), value);
		}

		/**
		 * \brief Set uniform variable with given location.
		 * \param location The location of the uniform variable. Must be valid.
		 * \param value New value of uniform
		 * \return true if successful
		 */
		static bool TryUniform(int location, const Mat4& value) noexcept;
		static bool TryUniform(int location, const Vec4& value) noexcept;
		static bool TryUniform(int location, const Vec3& value) noexcept;
		static bool TryUniform(int location, const Vec2& value) noexcept;
		static bool TryUniform(int location, float value) noexcept;
		
		/**
		 * \brief Get location of the uniform variable
		 * \param name The name of the uniform
		 * \return Location of the uniform or invalid_uniform_ if name is invalid
		 */
		[[nodiscard]] int GetUniformLocation(Name name) noexcept;
		[[nodiscard]] Path GetPath() const noexcept { return path_; }

		void swap(Shader& r) noexcept
		{
			using std::swap;
			swap(path_, r.path_);
			swap(vert_shader_, r.vert_shader_);
			swap(frag_shader_, r.frag_shader_);
			swap(shader_program_, r.shader_program_);
			swap(uniform_, r.uniform_);
		}

	private:
		friend std::hash<Shader>;

		Path path_;
		unsigned vert_shader_ = 0;
		unsigned frag_shader_ = 0;
		unsigned shader_program_ = 0;
		HashMap<Name, int> uniform_;
	};

	inline void swap(Shader& a, Shader& b) noexcept { a.swap(b); }
}

template <>
struct std::hash<oeng::Shader>
{
	size_t operator()(const oeng::Shader& key) const noexcept
	{
		return size_t(key.shader_program_);
	}
};
