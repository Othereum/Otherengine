#include "Shader.hpp"
#include <fstream>
#include <GL/glew.h>
#include "Format.hpp"
#include "Math.hpp"

namespace oeng
{
	std::string ReadFile(Path filepath)
	{
		std::ifstream file{ filepath, std::ios_base::in | std::ios_base::ate};
		if (!file.is_open()) throw std::ios_base::failure{format("Cannot read file. File not found: {}", filepath->string())};

		std::string code(file.tellg(), '\0');
		file.seekg(0);
		file.read(code.data(), code.size());
		return code;
	}
	
	static void CheckShader(unsigned shader)
	{
		auto is_valid = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &is_valid);
		if (!is_valid)
		{
			auto len = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
			std::string log(len, '\0');
			glGetShaderInfoLog(shader, len, nullptr, log.data());
			throw std::runtime_error{log};
		}
	}

	static void CheckProgram(unsigned program)
	{
		auto is_valid = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &is_valid);
		if (!is_valid)
		{
			auto len = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
			std::string log(len, '\0');
			glGetProgramInfoLog(program, len, nullptr, log.data());
			throw std::runtime_error{log};
		}
	}
	
	static unsigned Compile(Path file, unsigned type)
	{
		const auto code = ReadFile(file);
		const auto* const c_str = code.c_str();
		const auto shader = glCreateShader(type);
		glShaderSource(shader, 1, &c_str, nullptr);
		glCompileShader(shader);
		CheckShader(shader);
		return shader;
	}

	Shader::Shader(Path vert, Path frag):
		vert_shader_{Compile(vert, GL_VERTEX_SHADER)},
		frag_shader_{Compile(frag, GL_FRAGMENT_SHADER)},
		shader_program_{glCreateProgram()}
	{
		glAttachShader(shader_program_, vert_shader_);
		glAttachShader(shader_program_, frag_shader_);
		glLinkProgram(shader_program_);
		CheckProgram(shader_program_);
		Activate();
	}

	Shader::Shader(Shader&& r) noexcept
		:uniform_{std::move(r.uniform_)},
		vert_shader_{r.vert_shader_}, frag_shader_{r.frag_shader_}, shader_program_{r.shader_program_}
	{
		r.vert_shader_ = 0;
		r.frag_shader_ = 0;
		r.shader_program_ = 0;
	}

	Shader::~Shader()
	{
		// glDelete functions silently ignores 0 or invalid ID.
		glDeleteProgram(shader_program_);
		glDeleteShader(vert_shader_);
		glDeleteShader(frag_shader_);
	}

	void Shader::Activate() const
	{
		glUseProgram(shader_program_);
	}

	void Shader::SetMatrixUniform(Name name, const Mat4& matrix)
	{
		const auto loc = GetUniformLocation(name);
		glUniformMatrix4fv(loc, 1, true, matrix.AsFlatArr());
	}

	void Shader::SetTransform(const Mat4& matrix)
	{
		static const Name name = "uWorldTransform";
		SetMatrixUniform(name, matrix);
	}

	void Shader::SetViewProj(const Mat4& matrix)
	{
		static const Name name = "uViewProj";
		SetMatrixUniform(name, matrix);
	}

	int Shader::GetUniformLocation(Name name)
	{
		if (const auto found = uniform_.find(name); found != uniform_.end())
			return found->second;

		auto loc = glGetUniformLocation(shader_program_, name.CStr());
		uniform_.emplace(name, loc);
		return loc;
	}
}
