#pragma once
#include "Path.hpp"
#include "Name.hpp"
#include "Templates/HashMap.hpp"

namespace oeng
{
	class OpenGlError;
	
	class OEAPI Shader
	{
	public:
		static constexpr int invalid_uniform_ = -1;
		
		explicit Shader(Path path);
		~Shader();

		Shader(Shader&& r) noexcept;
		Shader& operator=(Shader&& r) noexcept;

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		std::strong_ordering operator<=>(const Shader& r) const noexcept
		{
			return shader_program_ <=> r.shader_program_;
		}

		[[nodiscard]] Path GetPath() const noexcept { return path_; }

		void Activate() const;
		
		/**
		 * \brief Set uniform variable of this shader with given name.
		 * \param name The name of the uniform variable. If is invalid, does nothing.
		 * \param value The new value to be set.
		 */
		template <class T>
		void SetUniform(Name name, const T& value)
		{
			SetUniform(GetUniformLocation(name), value);
		}
		
		/**
		 * \brief Set uniform variable of this shader with given location.
		 * \param location The location of the uniform variable. Does nothing if is invalid_uniform_. Otherwise, It must be valid location.
		 * \throw OpenGlError If location is invalid and not invalid_uniform_
		 */
		void SetUniform(int location, const Mat4& matrix);
		void SetUniform(int location, const Vec4& vector);
		void SetUniform(int location, const Vec3& vector);
		void SetUniform(int location, const Vec2& vector);
		void SetUniform(int location, float value);
		
		[[nodiscard]] int GetUniformLocation(Name name) const noexcept;

	private:
		friend std::hash<Shader>;

		void InvalidUniform(Name name) const;
		
		Path path_;
		unsigned vert_shader_;
		unsigned frag_shader_;
		unsigned shader_program_;
		mutable HashMap<Name, int> uniform_;
	};
}

template <>
struct std::hash<oeng::Shader>
{
	size_t operator()(const oeng::Shader& key) const noexcept
	{
		return size_t(key.shader_program_);
	}
};
