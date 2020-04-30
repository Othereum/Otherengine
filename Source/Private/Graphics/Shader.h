#pragma once
#include <unordered_map>
#include "Math.hpp"
#include "Name.h"

namespace oeng::graphics
{
	class Shader
	{
	public:
		Shader(std::string_view vert_name, std::string_view frag_name);
		~Shader();

		void SetMatrixUniform(Name name, const Mat4& matrix);

	private:
		int GetUniformLocation(Name name);
		
		std::unordered_map<Name, int> uniform_;
		unsigned vert_shader_;
		unsigned frag_shader_;
		unsigned shader_program_;
	};
}
