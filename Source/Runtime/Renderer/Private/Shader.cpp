#include "Shader.hpp"
#include <fstream>
#include "Math.hpp"
#include "OpenGL.hpp"

namespace oeng
{
	std::string ReadFile(const std::filesystem::path& path)
	{
		std::ifstream file{ path, std::ios_base::in | std::ios_base::ate};
		if (!file.is_open()) throw std::ios_base::failure{format("Cannot read file. File not found: {}", path.string())};

		std::string code(file.tellg(), '\0');
		file.seekg(0);
		file.read(code.data(), code.size());
		return code;
	}
	
	static void CheckShader(unsigned shader)
	{
		auto is_valid = 0;
		gl(glGetShaderiv, shader, GL_COMPILE_STATUS, &is_valid);
		if (!is_valid)
		{
			auto len = 0;
			gl(glGetShaderiv, shader, GL_INFO_LOG_LENGTH, &len);
			
			std::string log(len, '\0');
			gl(glGetShaderInfoLog, shader, len, nullptr, log.data());
			
			throw std::runtime_error{log};
		}
	}

	static void CheckProgram(unsigned program)
	{
		auto is_valid = 0;
		gl(glGetProgramiv, program, GL_LINK_STATUS, &is_valid);
		if (!is_valid)
		{
			auto len = 0;
			gl(glGetProgramiv, program, GL_INFO_LOG_LENGTH, &len);
			
			std::string log(len, '\0');
			gl(glGetProgramInfoLog, program, len, nullptr, log.data());
			
			throw std::runtime_error{log};
		}
	}
	
	static unsigned Compile(const std::filesystem::path& file, unsigned type)
	{
		const auto code = ReadFile(file);
		const auto* const c_str = code.c_str();
		const auto shader = gl(glCreateShader, type);
		gl(glShaderSource, shader, 1, &c_str, nullptr);
		gl(glCompileShader, shader);
		CheckShader(shader);
		return shader;
	}

	static auto Ext(std::filesystem::path path, const char* ext)
	{
		return path += ext;
	}

	Shader::Shader(Path path)
		:Asset{path},
		vert_shader_{Compile(Ext(path, ".vert"), GL_VERTEX_SHADER)},
		frag_shader_{Compile(Ext(path, ".frag"), GL_FRAGMENT_SHADER)},
		shader_program_{gl(glCreateProgram)}
	{
		gl(glAttachShader, shader_program_, vert_shader_);
		gl(glAttachShader, shader_program_, frag_shader_);
		gl(glLinkProgram, shader_program_);
		CheckProgram(shader_program_);
		Activate();
	}

	Shader::~Shader()
	{
		unsigned err;
		// glDelete functions silently ignores 0.
		gl(err, glDeleteProgram, shader_program_);
		gl(err, glDeleteShader, vert_shader_);
		gl(err, glDeleteShader, frag_shader_);
	}

	void Shader::Activate() const
	{
		gl(glUseProgram, shader_program_);
	}

	bool Shader::TryUniform(int location, const Mat4& value) noexcept
	{
		unsigned err;
		gl(err, glUniformMatrix4fv, location, 1, true, value.AsFlatArr());
		return !err;
	}

	bool Shader::TryUniform(int location, const Vec4& value) noexcept
	{
		unsigned err;
		gl(err, glUniform4fv, location, 1, value.data);
		return !err;
	}

	bool Shader::TryUniform(int location, const Vec3& value) noexcept
	{
		unsigned err;
		gl(err, glUniform3fv, location, 1, value.data);
		return !err;
	}

	bool Shader::TryUniform(int location, const Vec2& value) noexcept
	{
		unsigned err;
		gl(err, glUniform2fv, location, 1, value.data);
		return !err;
	}

	bool Shader::TryUniform(int location, float value) noexcept
	{
		unsigned err;
		gl(err, glUniform1f, location, value);
		return !err;
	}

	int Shader::GetUniformLocation(Name name) noexcept
	{
		if (const auto found = uniform_.find(name); found != uniform_.end())
			return found->second;

		unsigned err;
		const auto loc = gl(err, glGetUniformLocation, shader_program_, name->c_str());
		if (loc != invalid_uniform_) uniform_.try_emplace(name, loc);
		return loc;
	}
}
