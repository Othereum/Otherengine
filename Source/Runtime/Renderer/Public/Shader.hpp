#pragma once
#include "Name.hpp"
#include "Path.hpp"
#include "Templates/HashMap.hpp"

namespace oeng
{
	class OEAPI Shader
	{
	public:
		Shader(Path vert, Path frag);
		Shader(Shader&& r) noexcept;
		~Shader();

		Shader& operator=(Shader&& r) noexcept;
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		std::strong_ordering operator<=>(const Shader& r) const noexcept
		{
			return shader_program_ <=> r.shader_program_;
		}

		void Activate() const;
		void SetMatrixUniform(Name name, const Mat4& matrix);
		void SetTransform(const Mat4& matrix);
		void SetViewProj(const Mat4& matrix);

	private:
		friend std::hash<Shader>;
		[[nodiscard]] int GetUniformLocation(Name name);

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
