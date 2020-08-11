#include "Shader.hpp"
#include <fstream>
#include <GL/glew.h>
#include "Renderer.hpp"

namespace oeng::renderer
{
	std::string ReadFile(const std::filesystem::path& path)
	{
		std::ifstream file{ path, std::ios_base::in | std::ios_base::ate};
		if (!file.is_open()) Throw<std::ios_base::failure>(u8"Cannot read file. File not found: {}"sv, path.string<char8_t>(PoolAllocator<char8_t>{}));

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
	
	static unsigned Compile(const std::filesystem::path& file, unsigned type)
	{
		const auto code = ReadFile(file);
		const auto* const c_str = code.c_str();
		const auto shader = glCreateShader(type);
		glShaderSource(shader, 1, &c_str, nullptr);
		glCompileShader(shader);
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
		shader_program_{glCreateProgram()}
	{
		glAttachShader(shader_program_, vert_shader_);
		glAttachShader(shader_program_, frag_shader_);
		glLinkProgram(shader_program_);
		CheckProgram(shader_program_);
		Activate();
	}

	Shader::~Shader()
	{
		glDeleteProgram(shader_program_);
		glDeleteShader(vert_shader_);
		glDeleteShader(frag_shader_);
	}

	void Shader::Activate() const
	{
		glUseProgram(shader_program_);
	}

	template <class... Args, std::invocable<int, Args...> Fn>
	static bool GlUniform(Fn fn, int location, Args... args) noexcept
	{
		fn(location, args...);
		return glGetError() == GL_NO_ERROR;
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

	bool Shader::SetUniform(int location, const Uniform& value)
	{
		SCOPE_COUNTER(SetUniform);

		if (location == invalid_uniform)
		{
			OE_LOG(kRenderer, kWarn, u8"Attempted to set uniform with invalid location -1"sv);
			return false;
		}

		const auto cache = uniform_cache_.find(location);
		if (cache != uniform_cache_.end())
		{
			auto equals = []<class T1, class T2>(const T1& a, const T2& b)
			{
				if constexpr (!std::is_same_v<T1, T2>) { return false; }
				else { return IsNearlyEqual(a, b); }
			};
			if (std::visit(equals, cache->second, value)) return true;
		}
		
		SCOPE_COUNTER(RealSetUniform);
		const auto success = std::visit([&](const auto& v) { return GlUniform(location, v); }, value);
		if (success) uniform_cache_.insert_or_assign(cache, location, value);
		return success;
	}

	int Shader::GetUniformLocation(Name name) noexcept
	{
		if (const auto found = loc_cache_.find(name); found != loc_cache_.end())
			return found->second;

		const auto loc = glGetUniformLocation(shader_program_, AsString(name->c_str()));
		if (loc != invalid_uniform) loc_cache_.try_emplace(name, loc);
		return loc;
	}
}
