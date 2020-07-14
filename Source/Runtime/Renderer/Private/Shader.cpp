#include "Shader.hpp"
#include <fstream>
#include "Math.hpp"
#include "OpenGL.hpp"
#include "Stat.hpp"

namespace oeng
{
	std::string ReadFile(const std::filesystem::path& path)
	{
		std::ifstream file{ path, std::ios_base::in | std::ios_base::ate};
		if (!file.is_open()) Throw<std::ios_base::failure>(u8"Cannot read file. File not found: {}", path.u8string());

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

	static auto Ext(std::filesystem::path path, const char8_t* ext)
	{
		return path += ext;
	}

	Shader::Shader(Path path)
		:Asset{path},
		vert_shader_{Compile(Ext(path, u8".vert"), GL_VERTEX_SHADER)},
		frag_shader_{Compile(Ext(path, u8".frag"), GL_FRAGMENT_SHADER)},
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
		GL(glDeleteProgram, shader_program_);
		GL(glDeleteShader, vert_shader_);
		GL(glDeleteShader, frag_shader_);
	}

	void Shader::Activate() const
	{
		gl(glUseProgram, shader_program_);
	}

	template <class... Args, std::invocable<int, Args...> Fn>
	static bool GlUniform(Fn fn, int location, Args... args) noexcept
	{
		unsigned err;
		gl(err, fn, location, args...);
		return err == GL_NO_ERROR;
	}

	template <class Fn, size_t Row, size_t Col>
	static bool GlUniform(Fn fn, int l, const Matrix<Float, Row, Col>& v) noexcept
	{
		return GlUniform(fn, l, 1, true, v.AsFlatArr());
	}
	
	static bool GlUniform(int l, const Matrix<Float, 2, 2>& v) noexcept { return GlUniform(glUniformMatrix2fv, l, v); }
	static bool GlUniform(int l, const Matrix<Float, 2, 3>& v) noexcept { return GlUniform(glUniformMatrix2x3fv, l, v); }
	static bool GlUniform(int l, const Matrix<Float, 2, 4>& v) noexcept { return GlUniform(glUniformMatrix2x4fv, l, v); }
	static bool GlUniform(int l, const Matrix<Float, 3, 2>& v) noexcept { return GlUniform(glUniformMatrix3x2fv, l, v); }
	static bool GlUniform(int l, const Matrix<Float, 3, 3>& v) noexcept { return GlUniform(glUniformMatrix3fv, l, v); }
	static bool GlUniform(int l, const Matrix<Float, 3, 4>& v) noexcept { return GlUniform(glUniformMatrix3x4fv, l, v); }
	static bool GlUniform(int l, const Matrix<Float, 4, 2>& v) noexcept { return GlUniform(glUniformMatrix4x2fv, l, v); }
	static bool GlUniform(int l, const Matrix<Float, 4, 3>& v) noexcept { return GlUniform(glUniformMatrix4x3fv, l, v); }
	static bool GlUniform(int l, const Matrix<Float, 4, 4>& v) noexcept { return GlUniform(glUniformMatrix4fv, l, v); }

	static bool GlUniform(int l, Vec2 v) noexcept { return GlUniform(glUniform2f, l, v[0], v[1]); }
	static bool GlUniform(int l, Vec3 v) noexcept { return GlUniform(glUniform3f, l, v[0], v[1], v[2]); }
	static bool GlUniform(int l, Vec4 v) noexcept { return GlUniform(glUniform4f, l, v[0], v[1], v[2], v[3]); }
	static bool GlUniform(int l, Vector<int, 2> v) noexcept { return GlUniform(glUniform2i, l, v[0], v[1]); }
	static bool GlUniform(int l, Vector<int, 3> v) noexcept { return GlUniform(glUniform3i, l, v[0], v[1], v[2]); }
	static bool GlUniform(int l, Vector<int, 4> v) noexcept { return GlUniform(glUniform4i, l, v[0], v[1], v[2], v[3]); }

	static bool GlUniform(int l, float v) noexcept { return GlUniform(glUniform1f, l, v); }
	static bool GlUniform(int l, int v) noexcept { return GlUniform(glUniform1i, l, v); }

	bool Shader::TryUniform(int location, const Uniform& value)
	{
		if (location == invalid_uniform_) return false;
		ScopeCounter counter{u8"SetUniform"};

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
		const auto loc = gl(err, glGetUniformLocation, shader_program_, AsString(name->c_str()));
		if (loc != invalid_uniform_) loc_cache_.try_emplace(name, loc);
		return loc;
	}
}
