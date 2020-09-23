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
    LoadUniforms();
}

void OpenGLShader::Activate() const noexcept
{
    glUseProgram(*program_);
}

void OpenGLShader::LinkProgram()
{
    glAttachShader(*program_, *vertex_);
    glAttachShader(*program_, *frag_);
    glLinkProgram(*program_);
    CheckProgram(*program_);
}

void OpenGLShader::LoadUniforms()
{
    int count;
    glGetProgramInterfaceiv(*program_, GL_UNIFORM, GL_ACTIVE_RESOURCES, &count);

    for (auto idx = 0, tex = 0; idx < count; ++idx)
    {
        constexpr unsigned props[]{GL_TYPE, GL_LOCATION, GL_NAME_LENGTH};
        int values[3];
        glGetProgramResourceiv(*program_, GL_UNIFORM, idx, 3, props, 3, nullptr, values);

        const auto& [type, loc, name_bufsz] = values;
        assert(loc != -1);

        const std::unique_ptr<char[]> name_ptr{new char[name_bufsz]};
        glGetProgramResourceName(*program_, GL_UNIFORM, idx, name_bufsz, nullptr, name_ptr.get());
        const std::u8string_view name{AsString8(name_ptr.get()), static_cast<size_t>(name_bufsz - 1)};

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

static void GlUniform(int location, Vec2 v) noexcept
{
    glUniform2f(location, v[0], v[1]);
}
static void GlUniform(int location, Vec3 v) noexcept
{
    glUniform3f(location, v[0], v[1], v[2]);
}
static void GlUniform(int location, Vec4 v) noexcept
{
    glUniform4f(location, v[0], v[1], v[2], v[3]);
}
static void GlUniform(int location, Vec2i v) noexcept
{
    glUniform2i(location, v[0], v[1]);
}
static void GlUniform(int location, Vec3i v) noexcept
{
    glUniform3i(location, v[0], v[1], v[2]);
}
static void GlUniform(int location, Vec4i v) noexcept
{
    glUniform4i(location, v[0], v[1], v[2], v[3]);
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

template <class T> bool OpenGLShader::ApplyUniform(Name name, T& value, const std::unordered_map<Name, int>& loc_cache)
{
    const auto location = loc_cache.find(name);
    if (location == loc_cache.end())
        return false;

    if (IsRedundant(name, value))
        return true;

    GlUniform(location->second, value);

    if (glGetError() != GL_NO_ERROR)
        return false;

    UpdateCache(name, value);
    return true;
}

bool OpenGLShader::ApplyParam(Name name, ScalarParam value)
{
    return ApplyUniform(name, value, scalar_loc_);
}

bool OpenGLShader::ApplyParam(Name name, const VectorParam& value)
{
    return ApplyUniform(name, value, vector_loc_);
}

bool OpenGLShader::ApplyParam(Name name, const MatrixParam& value)
{
    return ApplyUniform(name, value, matrix_loc_);
}

bool OpenGLShader::ApplyParam(Name name, RHITexture& value)
{
    return ApplyUniform(name, value, texture_idx_);
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
