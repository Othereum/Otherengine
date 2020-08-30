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
    : vertex_{Compile(vertex_shader, GL_VERTEX_SHADER)},
      frag_{Compile(frag_shader, GL_FRAGMENT_SHADER)},
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
        case GL_FLOAT:
            scalar_loc_.emplace(name, loc);
            break;

        case GL_FLOAT_VEC4:
            vector_loc_.emplace(name, loc);
            break;

        case GL_SAMPLER_2D:
            glUniform1i(loc, tex);
            texture_idx_.emplace(name, tex);
            ++tex;
            break;

        default: ;
        }
    }
}

template <class T, class Fn>
bool OpenGLShader::ApplyParam(Name name, const T& value, const std::unordered_map<Name, int>& loc_cache, Fn fn)
{
    const auto loc = loc_cache.find(name);
    if (loc == loc_cache.end())
        return false;

    if (IsRedundant(name, value))
        return true;

    fn(loc->second, value);

    if (glGetError() != GL_NO_ERROR)
        return false;

    UpdateCache(name, value);
    return true;
}

bool OpenGLShader::ApplyParam(Name name, Float value)
{
    return ApplyParam(name, value, scalar_loc_, glUniform1f);
}

bool OpenGLShader::ApplyParam(Name name, const Vec4& value)
{
    return ApplyParam(name, value, vector_loc_, [](int location, const Vec4& val)
    {
        glUniform4fv(location, 1, val.data);
    });
}

bool OpenGLShader::IsScalarParam(Name name) const
{
    return scalar_loc_.contains(name);
}

bool OpenGLShader::IsVectorParam(Name name) const
{
    return vector_loc_.contains(name);
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
}
}
