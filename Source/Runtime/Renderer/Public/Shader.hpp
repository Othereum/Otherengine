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
		void SetMatrixUniform(Name name, const Mat4& matrix);
		void SetMatrixUniform(int location, const Mat4& matrix) const;
		
		void SetTransform(const Mat4& matrix);
		void SetViewProj(const Mat4& matrix);
		[[nodiscard]] int GetUniformLocation(Name name);

	private:
		friend std::hash<Shader>;

		Path path_;
		HashMap<Name, int> uniform_;
		unsigned vert_shader_;
		unsigned frag_shader_;
		unsigned shader_program_;
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
