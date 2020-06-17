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

	static bool GlUniform(int location, const Mat4& value) noexcept
	{
		unsigned err;
		gl(err, glUniformMatrix4fv, location, 1, true, value.AsFlatArr());
		return err == GL_NO_ERROR;
	}

	static bool GlUniform(int location, Vec4 value) noexcept
	{
		unsigned err;
		gl(err, glUniform4f, location, value[0], value[1], value[2], value[3]);
		return err == GL_NO_ERROR;
	}

	static bool GlUniform(int location, Vec3 value) noexcept
	{
		unsigned err;
		gl(err, glUniform3f, location, value[0], value[1], value[2]);
		return err == GL_NO_ERROR;
	}

	static bool GlUniform(int location, Vec2 value) noexcept
	{
		unsigned err;
		gl(err, glUniform2f, location, value[0], value[1]);
		return err == GL_NO_ERROR;
	}

	static bool GlUniform(int location, float value) noexcept
	{
		unsigned err;
		gl(err, glUniform1f, location, value);
		return err == GL_NO_ERROR;
	}

	static bool GlUniform(int location, int32_t value) noexcept
	{
		unsigned err;
		gl(err, glUniform1i, location, value);
		return err == GL_NO_ERROR;
	}

	static bool GlUniform(int location, uint32_t value) noexcept
	{
		unsigned err;
		gl(err, glUniform1ui, location, value);
		return err == GL_NO_ERROR;
	}

	bool Shader::TryUniform(int location, const Uniform& value)
	{
		if (location == invalid_uniform_) return false;

		const auto cache = uniform_cache_.find(location);
		if (cache != uniform_cache_.end())
		{
			auto equals = []<class T0, class T1>(const T0& a, const T1& b)
			{
				if constexpr (!std::is_same_v<T0, T1>) { return false; }
				else { return IsNearlyEqual(a, b); }
			};
			if (std::visit(equals, cache->second, value)) return true;
		}

		const auto success = std::visit([&](const auto& v) { return GlUniform(location, v); }, value);
		if (success) uniform_cache_.insert_or_assign(cache, location, value);
		return success;
	}

	int Shader::GetUniformLocation(Name name) noexcept
	{
		if (const auto found = loc_cache_.find(name); found != loc_cache_.end())
			return found->second;

		unsigned err;
		const auto loc = gl(err, glGetUniformLocation, shader_program_, name->c_str());
		if (loc != invalid_uniform_) loc_cache_.try_emplace(name, loc);
		return loc;
	}
}
