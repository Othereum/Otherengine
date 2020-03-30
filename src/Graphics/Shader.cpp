#include "Graphics/Shader.h"
#include <fstream>
#include <GL/glew.h>
#include <fmt/core.h>

namespace game::graphics
{
	static unsigned Compile(std::string_view filename, unsigned type)
	{
		std::ifstream file{filename.data(), std::ios_base::in | std::ios_base::ate};
		if (!file.is_open()) throw std::ios_base::failure{fmt::format("Shader file not found: {}", filename)};

		std::string code(file.tellg(), 0);
		file.seekg(0);
		file.read(code.data(), code.size());

		const auto str = code.c_str();
		const auto shader = glCreateShader(type);
		glShaderSource(shader, 1, &str, nullptr);
		glCompileShader(shader);
	}
	
	Shader::Shader(std::string_view vert_name, std::string_view frag_name)
	{
	}

	Shader::~Shader()
	{
	}
}
