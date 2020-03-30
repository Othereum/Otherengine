#include "Graphics/Shader.h"
#include <fstream>
#include <GL/glew.h>
#include <fmt/core.h>

namespace game::graphics
{
	static void Check(unsigned shader)
	{
		int is_valid;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &is_valid);
		if (!is_valid)
		{
			int len;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
			std::string log(len, 0);
			glGetShaderInfoLog(shader, len, nullptr, log.data());
			throw std::runtime_error{log};
		}
	}
	
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
		Check(shader);
	}
	
	Shader::Shader(std::string_view vert_name, std::string_view frag_name)
	{
	}

	Shader::~Shader()
	{
	}
}
