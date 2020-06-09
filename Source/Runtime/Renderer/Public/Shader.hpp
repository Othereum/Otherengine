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


		void Activate() const;
		void SetMatrixUniform(Name name, const Mat4& matrix);
		void SetTransform(const Mat4& matrix);
		void SetViewProj(const Mat4& matrix);

	private:
		int GetUniformLocation(Name name);
		
		HashMap<Name, int> uniform_;
		unsigned vert_shader_;
		unsigned frag_shader_;
		unsigned shader_program_;
	};
}
