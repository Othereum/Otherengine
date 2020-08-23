#include "OpenGLShader.hpp"
#include <GL/glew.h>

namespace oeng
{
inline namespace opengldrv
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

[[nodiscard]] static ShaderHandle Compile(const char* code, unsigned type)
{
    ShaderHandle shader{glCreateShader(type)};
    glShaderSource(*shader, 1, &code, nullptr);
    glCompileShader(*shader);

    CheckShader(*shader);
    return shader;
}

OpenGLShader::OpenGLShader(const char* vertex_shader, const char* frag_shader)
    : vertex_shader_{Compile(vertex_shader, GL_VERTEX_SHADER)},
      frag_shader_{Compile(frag_shader, GL_FRAGMENT_SHADER)},
      shader_program_{glCreateProgram()}
{
    glAttachShader(*shader_program_, *vertex_shader_);
    glAttachShader(*shader_program_, *frag_shader_);
    glLinkProgram(*shader_program_);
    CheckProgram(*shader_program_);
    OpenGLShader::Activate();
}

void OpenGLShader::Activate() const noexcept
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

static constexpr int kInvalidUniform = -1;

bool OpenGLShader::SetParam(Name name, const ShaderParam& value)
{
    SCOPE_COUNTER(SetUniform);

    const auto location = GetUniformLocation(name);
    if (location == kInvalidUniform)
        return false;

    if (IsRedundant(name, value))
        return true;

    std::visit([&](const auto& v)
    {
        SCOPE_COUNTER(RealSetUniform);
        return GlUniform(location, v);
    }, value);

    if (glGetError() != GL_NO_ERROR)
        return false;

    UpdateCache(name, value);
    return true;
}

bool OpenGLShader::IsValidParam(Name name) const noexcept
{
    if (loc_cache_.contains(name))
        return true;

    return kInvalidUniform != glGetUniformLocation(*shader_program_, AsString(name->c_str()));
}

int OpenGLShader::GetUniformLocation(Name name)
{
    if (const auto found = loc_cache_.find(name); found != loc_cache_.end())
        return found->second;

    const auto loc = glGetUniformLocation(*shader_program_, AsString(name->c_str()));
    if (loc != kInvalidUniform)
        loc_cache_.emplace(name, loc);

    return loc;
}

void ShaderDeleter::operator()(unsigned id) const noexcept
{
    glDeleteShader(id);
}

void ProgramDeleter::operator()(unsigned id) const noexcept
{
    glDeleteProgram(id);
}
}
}
