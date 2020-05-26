#pragma once
#include <unordered_map>
#include "Math.hpp"
#include "Name.hpp"
#include "Path.hpp"

namespace oeng
{
	class Shader
	{
	public:
		Shader(Path vert, Path frag);
		~Shader();

		void Activate() const;
		void SetMatrixUniform(Name name, const Mat4& matrix);

	private:
		int GetUniformLocation(Name name);
		
		std::unordered_map<Name, int> uniform_;
		unsigned vert_shader_;
		unsigned frag_shader_;
		unsigned shader_program_;
	};
}
