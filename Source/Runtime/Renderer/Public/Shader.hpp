#pragma once
#include "Uniform.hpp"

namespace oeng::renderer
{
/**
 * Exception class indicating shader compilation error. `what()` returns the compilation error log.
 */
class ShaderCompileError : public std::runtime_error
{
    explicit ShaderCompileError(const std::string& message)
        : std::runtime_error{message}
    {
    }

    explicit ShaderCompileError(const char* message)
        : std::runtime_error{message}
    {
    }
};

class RENDERER_API Shader : public Asset
{
public:
    static constexpr int invalid_uniform = -1;

    Shader() = default;
    ~Shader();

    /**
     * Load shader from given path.
     * @param path Shader path. For example, "../Shaders/Phong" loads Phong.vert and Phong.frag.
     * @throw std::ios_base::failure If failed to open file.
     * @throw ShaderCompileError If compilation failed.
     */
    explicit Shader(Path path);

    Shader(Shader&& r) noexcept
        : Asset{std::move(r)}, vert_shader_{r.vert_shader_}, frag_shader_{r.frag_shader_},
          shader_program_{r.shader_program_}, loc_cache_{std::move(r.loc_cache_)},
          uniform_cache_{std::move(r.uniform_cache_)}
    {
        r.vert_shader_ = 0;
        r.frag_shader_ = 0;
        r.shader_program_ = 0;
    }

    void Load(Path path)
    {
        Shader{path}.swap(*this);
    }

    Shader& operator=(Shader&& r) noexcept
    {
        Shader{std::move(r)}.swap(*this);
        return *this;
    }

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    std::strong_ordering operator<=>(const Shader& r) const noexcept
    {
        return shader_program_ <=> r.shader_program_;
    }

    void Activate() const;

    /**
     * Try to set uniform variable of this shader with given name.
     * If the name is not valid, it returns false quietly.
     * Otherwise, error will be logged if occurs.
     * @param name The name of the uniform variable.
     * @param value The new value to be set.
     * @return True if successful.
     */
    bool TryUniform(Name name, const Uniform& value)
    {
        const auto location = GetUniformLocation(name);

        if (location == invalid_uniform)
            return false;

        return SetUniform(location, value);
    }

    /**
     * Set uniform variable of this shader with given name.
     * An error will be logged if occurs.
     * @param name The name of the uniform variable. Must be valid.
     * @param value The new value to be set.
     * @return True if successful.
     */
    bool SetUniform(Name name, const Uniform& value)
    {
        return SetUniform(GetUniformLocation(name), value);
    }

    /**
     * Set uniform variable of this shader with given location.
     * An error will be logged if occurs.
     * @param location The location of the uniform variable. Must be valid.
     * @param value The new value to be set.
     * @return True if successful.
     */
    bool SetUniform(int location, const Uniform& value);

    /**
     * Get location of the uniform variable
     * @param name The name of the uniform
     * @return Location of the uniform or invalid_uniform if name is invalid
     */
    [[nodiscard]] int GetUniformLocation(Name name) noexcept;

    void swap(Shader& r) noexcept
    {
        Asset::swap(r);
        using std::swap;
        swap(vert_shader_, r.vert_shader_);
        swap(frag_shader_, r.frag_shader_);
        swap(shader_program_, r.shader_program_);
        swap(loc_cache_, r.loc_cache_);
        swap(uniform_cache_, r.uniform_cache_);
    }

private:
    friend std::hash<Shader>;

    unsigned vert_shader_ = 0;
    unsigned frag_shader_ = 0;
    unsigned shader_program_ = 0;
    std::unordered_map<Name, int> loc_cache_;
    std::unordered_map<int, Uniform> uniform_cache_;
};

inline void swap(Shader& a, Shader& b) noexcept
{
    a.swap(b);
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
template <>
struct std::hash<oeng::renderer::Shader>
{
    size_t operator()(const oeng::renderer::Shader& key) const noexcept
    {
        return size_t(key.shader_program_);
    }
};
#endif
