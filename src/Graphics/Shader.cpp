#include "Graphics/Shader.h"
#include <fstream>
#include <GL/glew.h>
#include <fmt/core.h>

namespace game::graphics
{
	static std::string Open(std::string_view filename)
	{
		std::ifstream file{filename.data(), std::ios_base::in | std::ios_base::ate};
		if (!file.is_open()) throw std::ios_base::failure{fmt::format("Shader file not found: '{}'", filename)};

		std::string code(file.tellg(), 0);
		file.seekg(0);
		file.read(code.data(), code.size());
		return code;
	}
	
	static void CheckShader(unsigned shader)
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

	static void CheckProgram(unsigned program)
	{
		int is_valid;
		glGetProgramiv(program, GL_LINK_STATUS, &is_valid);
		if (!is_valid)
		{
			int len;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
			std::string log(len, 0);
			glGetProgramInfoLog(program, len, nullptr, log.data());
			throw std::runtime_error{log};
		}
	}
	
	static unsigned Compile(std::string_view filename, unsigned type)
	{
		const auto str = Open(filename);
		const auto c_str = str.c_str();
		const auto shader = glCreateShader(type);
		glShaderSource(shader, 1, &c_str, nullptr);
		glCompileShader(shader);
		CheckShader(shader);
		return shader;
	}

	Shader::Shader(std::string_view vert_name, std::string_view frag_name):
		vert_shader_{Compile(vert_name, GL_VERTEX_SHADER)},
		frag_shader_{Compile(frag_name, GL_FRAGMENT_SHADER)},
		shader_program_{glCreateProgram()}
	{
		glAttachShader(shader_program_, vert_shader_);
		glAttachShader(shader_program_, frag_shader_);
		glLinkProgram(shader_program_);
		CheckProgram(shader_program_);
		glUseProgram(shader_program_);
	}

	Shader::~Shader()
	{
		glDeleteProgram(shader_program_);
		glDeleteShader(vert_shader_);
		glDeleteShader(frag_shader_);
	}
}
