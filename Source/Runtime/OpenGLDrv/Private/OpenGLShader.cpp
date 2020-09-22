#include "OpenGLShader.hpp"
#include "RHITexture.hpp"
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
    : vertex_{Compile(vertex_shader, GL_VERTEX_SHADER)}, frag_{Compile(frag_shader, GL_FRAGMENT_SHADER)},
      program_{glCreateProgram()}
{
    LinkProgram();
    Activate();
    Init();
}

void OpenGLShader::Activate() const noexcept
{
    glUseProgram(*program_);
}

void OpenGLShader::LinkProgram()
{
    glAttachShader(*program_, *program_);
    glAttachShader(*program_, *frag_);
    glLinkProgram(*program_);
    CheckProgram(*program_);
}

static constexpr int kInvalidUniform = -1;

void OpenGLShader::Init()
{
    int count;
    glGetProgramiv(*program_, GL_ACTIVE_UNIFORMS, &count);

    int name_buf_size;
    glGetProgramiv(*program_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &name_buf_size);
    const std::unique_ptr<char8_t[]> name_ptr{new char8_t[name_buf_size]};

    for (auto uniform = 0, tex = 0; uniform < count; ++uniform)
    {
        int name_len, size;
        unsigned type;
        glGetActiveUniform(*program_, uniform, name_buf_size, &name_len, &size, &type, AsString(name_ptr.get()));

        const auto loc = glGetUniformLocation(*program_, AsString(name_ptr.get()));
        assert(loc != kInvalidUniform);

        const std::u8string_view name{name_ptr.get(), size_t(name_len)};

        switch (type)
        {
        case GL_INT:
        case GL_FLOAT:
            scalar_loc_.emplace(name, loc);
            break;

        case GL_FLOAT_VEC2:
        case GL_FLOAT_VEC3:
        case GL_FLOAT_VEC4:
        case GL_INT_VEC2:
        case GL_INT_VEC3:
        case GL_INT_VEC4:
            vector_loc_.emplace(name, loc);
            break;

        case GL_FLOAT_MAT2:
        case GL_FLOAT_MAT3:
        case GL_FLOAT_MAT4:
        case GL_FLOAT_MAT2x3:
        case GL_FLOAT_MAT2x4:
        case GL_FLOAT_MAT3x2:
        case GL_FLOAT_MAT3x4:
        case GL_FLOAT_MAT4x2:
        case GL_FLOAT_MAT4x3:
            matrix_loc_.emplace(name, loc);
            break;

        case GL_SAMPLER_2D:
            glUniform1i(loc, tex);
            texture_idx_.emplace(name, tex);
            ++tex;
            break;
        }
    }
}

static void GlUniform(int location, Float value) noexcept
{
    glUniform1f(location, value);
}
static void GlUniform(int location, int32_t value) noexcept
{
    glUniform1i(location, value);
}

static void GlUniform(int location, const Vec2& value) noexcept
{
    glUniform2fv(location, 1, value.data);
}
static void GlUniform(int location, const Vec3& value) noexcept
{
    glUniform3fv(location, 1, value.data);
}
static void GlUniform(int location, const Vec4& value) noexcept
{
    glUniform4fv(location, 1, value.data);
}
static void GlUniform(int location, const Vec2i& value) noexcept
{
    glUniform2iv(location, 1, value.data);
}
static void GlUniform(int location, const Vec3i& value) noexcept
{
    glUniform3iv(location, 1, value.data);
}
static void GlUniform(int location, const Vec4i& value) noexcept
{
    glUniform4iv(location, 1, value.data);
}

static void GlUniform(int location, const Mat2& value) noexcept
{
    glUniformMatrix2fv(location, 1, true, value.AsFlatArr());
}
static void GlUniform(int location, const Mat3& value) noexcept
{
    glUniformMatrix3fv(location, 1, true, value.AsFlatArr());
}
static void GlUniform(int location, const Mat4& value) noexcept
{
    glUniformMatrix4fv(location, 1, true, value.AsFlatArr());
}
static void GlUniform(int location, const Mat2x3& value) noexcept
{
    glUniformMatrix2x3fv(location, 1, true, value.AsFlatArr());
}
static void GlUniform(int location, const Mat2x4& value) noexcept
{
    glUniformMatrix2x4fv(location, 1, true, value.AsFlatArr());
}
static void GlUniform(int location, const Mat3x2& value) noexcept
{
    glUniformMatrix3x2fv(location, 1, true, value.AsFlatArr());
}
static void GlUniform(int location, const Mat3x4& value) noexcept
{
    glUniformMatrix3x4fv(location, 1, true, value.AsFlatArr());
}
static void GlUniform(int location, const Mat4x2& value) noexcept
{
    glUniformMatrix4x2fv(location, 1, true, value.AsFlatArr());
}
static void GlUniform(int location, const Mat4x3& value) noexcept
{
    glUniformMatrix4x3fv(location, 1, true, value.AsFlatArr());
}

static void GlUniform(int index, RHITexture& value) noexcept
{
    glActiveTexture(GL_TEXTURE0 + index);
    value.Activate();
}

static void GlUniform(int location, ScalarParam value) noexcept
{
    std::visit([location](auto val) { GlUniform(location, val); }, value);
}

static void GlUniform(int location, const VectorParam& value) noexcept
{
    std::visit([location](auto val) { GlUniform(location, val); }, value);
}

static void GlUniform(int location, const MatrixParam& value) noexcept
{
    std::visit([location](auto val) { GlUniform(location, val); }, value);
}

template <class T>
bool OpenGLShader::ApplyParam(Name name, T&& value, const std::unordered_map<Name, int>& loc_cache)
{
    const auto loc = loc_cache.find(name);
    if (loc == loc_cache.end())
        return false;

    if (IsRedundant(name, value))
        return true;

    GlUniform(loc->second, value);

    if (glGetError() != GL_NO_ERROR)
        return false;

    UpdateCache(name, value);
    return true;
}

bool OpenGLShader::ApplyParam(Name name, ScalarParam value)
{
    return ApplyParam(name, value, scalar_loc_);
}

bool OpenGLShader::ApplyParam(Name name, const VectorParam& value)
{
    return ApplyParam(name, value, vector_loc_);
}

bool OpenGLShader::ApplyParam(Name name, const MatrixParam& value)
{
    return ApplyParam(name, value, matrix_loc_);
}

bool OpenGLShader::ApplyParam(Name name, RHITexture& value)
{
    return ApplyParam(name, value, texture_idx_);
}

bool OpenGLShader::IsScalarParam(Name name) const
{
    return scalar_loc_.contains(name);
}

bool OpenGLShader::IsVectorParam(Name name) const
{
    return vector_loc_.contains(name);
}

bool OpenGLShader::IsMatrixParam(Name name) const
{
    return matrix_loc_.contains(name);
}

bool OpenGLShader::IsTextureParam(Name name) const
{
    return texture_idx_.contains(name);
}

void ShaderDeleter::operator()(unsigned id) const noexcept
{
    glDeleteShader(id);
}

void ProgramDeleter::operator()(unsigned id) const noexcept
{
    glDeleteProgram(id);
}
} // namespace opengldrv
} // namespace oeng
