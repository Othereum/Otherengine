#include "Shader.hpp"
#include "Renderer.hpp"
#include <fstream>
#include <GL/glew.h>

namespace oeng::renderer
{
template <class GetParam, class GetInfoLog>
static void CheckValid(unsigned object, GetParam get_param, unsigned status_param_name, GetInfoLog get_info_log)
{
    int is_valid;
    get_param(object, status_param_name, &is_valid);

    if (is_valid == GL_FALSE)
    {
        int buf_sz;
        get_param(object, GL_INFO_LOG_LENGTH, &buf_sz);

        int len;
        std::string log(buf_sz - 1, '\0');
        get_info_log(object, buf_sz, &len, log.data());
        assert(buf_sz == len + 1);

        throw ShaderCompileError{log};
    }
}

static void CheckShader(unsigned shader)
{
    CheckValid(shader, glGetShaderiv, GL_COMPILE_STATUS, glGetShaderInfoLog);
}

static void CheckProgram(unsigned program)
{
    CheckValid(program, glGetProgramiv, GL_LINK_STATUS, glGetProgramInfoLog);
}

[[nodiscard]] static fs::path Ext(fs::path path, std::u8string_view ext)
{
    path += ext;
    return path;
}

Shader::Shader(Path path)
    : Asset{path},
      vert_shader_{Compile(Ext(path, u8".vert"sv), GL_VERTEX_SHADER)},
      frag_shader_{Compile(Ext(path, u8".frag"sv), GL_FRAGMENT_SHADER)},
      shader_program_{glCreateProgram()}
{
    glAttachShader(*shader_program_, *vert_shader_);
    glAttachShader(*shader_program_, *frag_shader_);
    glLinkProgram(*shader_program_);
    CheckProgram(*shader_program_);
    Activate();
}

void Shader::Activate() const
{
    glUseProgram(*shader_program_);
}

static void GlUniform(int l, const Matrix<Float, 2, 2>& v) noexcept
{
    glUniformMatrix2fv(l, 1, true, v.AsFlatArr());
}

static void GlUniform(int l, const Matrix<Float, 2, 3>& v) noexcept
{
    glUniformMatrix2x3fv(l, 1, true, v.AsFlatArr());
}

static void GlUniform(int l, const Matrix<Float, 2, 4>& v) noexcept
{
    glUniformMatrix2x4fv(l, 1, true, v.AsFlatArr());
}

static void GlUniform(int l, const Matrix<Float, 3, 2>& v) noexcept
{
    glUniformMatrix3x2fv(l, 1, true, v.AsFlatArr());
}

static void GlUniform(int l, const Matrix<Float, 3, 3>& v) noexcept
{
    glUniformMatrix3fv(l, 1, true, v.AsFlatArr());
}

static void GlUniform(int l, const Matrix<Float, 3, 4>& v) noexcept
{
    glUniformMatrix3x4fv(l, 1, true, v.AsFlatArr());
}

static void GlUniform(int l, const Matrix<Float, 4, 2>& v) noexcept
{
    glUniformMatrix4x2fv(l, 1, true, v.AsFlatArr());
}

static void GlUniform(int l, const Matrix<Float, 4, 3>& v) noexcept
{
    glUniformMatrix4x3fv(l, 1, true, v.AsFlatArr());
}

static void GlUniform(int l, const Matrix<Float, 4, 4>& v) noexcept
{
    glUniformMatrix4fv(l, 1, true, v.AsFlatArr());
}

static void GlUniform(int l, Vec2 v) noexcept
{
    glUniform2f(l, v[0], v[1]);
}

static void GlUniform(int l, Vec3 v) noexcept
{
    glUniform3f(l, v[0], v[1], v[2]);
}

static void GlUniform(int l, Vec4 v) noexcept
{
    glUniform4f(l, v[0], v[1], v[2], v[3]);
}

static void GlUniform(int l, Vector<int, 2> v) noexcept
{
    glUniform2i(l, v[0], v[1]);
}

static void GlUniform(int l, Vector<int, 3> v) noexcept
{
    glUniform3i(l, v[0], v[1], v[2]);
}

static void GlUniform(int l, Vector<int, 4> v) noexcept
{
    glUniform4i(l, v[0], v[1], v[2], v[3]);
}

static void GlUniform(int l, float v) noexcept
{
    glUniform1f(l, v);
}

static void GlUniform(int l, int v) noexcept
{
    glUniform1i(l, v);
}

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
            if constexpr (!std::is_same_v<T1, T2>)
            {
                return false;
            }
            else
            {
                return IsNearlyEqual(a, b);
            }
        };

        if (std::visit(equals, cache->second, value))
            return true;
    }

    std::visit([&](const auto& v)
    {
        SCOPE_COUNTER(RealSetUniform);
        return GlUniform(location, v);
    }, value);

    if (glGetError() != GL_NO_ERROR)
        return false;

    uniform_cache_.insert_or_assign(cache, location, value);
    return true;
}

int Shader::GetUniformLocation(Name name) noexcept
{
    if (const auto found = loc_cache_.find(name); found != loc_cache_.end())
        return found->second;

    const auto loc = glGetUniformLocation(*shader_program_, AsString(name->c_str()));

    if (loc != invalid_uniform)
        loc_cache_.try_emplace(name, loc);

    return loc;
}

void Shader::ShaderDeleter::operator()(unsigned id) const noexcept
{
    glDeleteShader(id);
}

void Shader::ProgramDeleter::operator()(unsigned id) const noexcept
{
    glDeleteProgram(id);
}

[[nodiscard]] std::string ReadFile(const fs::path& path)
{
    std::ifstream file{path, std::ios_base::in | std::ios_base::ate};

    if (!file.is_open())
        throw std::ios_base::failure{fmt::format("Cannot read file. File not found: {}"sv, path.string())};

    std::string code(file.tellg(), '\0');
    file.seekg(0);
    file.read(code.data(), code.size());
    return code;
}

Resource<unsigned, Shader::ShaderDeleter> Shader::Compile(const fs::path& file, unsigned type)
{
    const auto code = ReadFile(file);
    const auto* const c_str = code.c_str();

    Resource<unsigned, ShaderDeleter> shader{glCreateShader(type)};
    glShaderSource(*shader, 1, &c_str, nullptr);
    glCompileShader(*shader);

    CheckShader(*shader);
    return shader;
}
}
