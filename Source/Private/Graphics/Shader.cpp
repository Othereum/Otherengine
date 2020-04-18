#include "Graphics/Shader.h"
#include <GL/glew.h>
#include <stdexcept>
#include "StringUtil.hpp"

namespace oeng::graphics
{
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
		const auto code = str::ReadFile(filename);
		const auto c_str = code.c_str();
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
