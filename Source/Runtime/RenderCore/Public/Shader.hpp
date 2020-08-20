#pragma once
#include "Uniform.hpp"
#include "Templates/Resource.hpp"

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

    /**
     * Load shader from given path.
     * @param path Shader path. For example, "../Shaders/Phong" loads Phong.vert and Phong.frag.
     * @throw std::ios_base::failure If failed to open file.
     * @throw ShaderCompileError If compilation failed.
     */
    explicit Shader(Path path);

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

    [[nodiscard]] size_t hash_value() const noexcept
    {
        return *shader_program_;
    }

private:
    struct ShaderDeleter
    {
        void operator()(unsigned id) const noexcept;
    };

    struct ProgramDeleter
    {
        void operator()(unsigned id) const noexcept;
    };

    [[nodiscard]] static Resource<unsigned, ShaderDeleter> Compile(const fs::path& file, unsigned type);

    Resource<unsigned, ShaderDeleter> vert_shader_;
    Resource<unsigned, ShaderDeleter> frag_shader_;
    Resource<unsigned, ProgramDeleter> shader_program_;
    std::unordered_map<Name, int> loc_cache_;
    std::unordered_map<int, Uniform> uniform_cache_;
};
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
template <>
struct std::hash<oeng::renderer::Shader>
{
    [[nodiscard]] size_t operator()(const oeng::renderer::Shader& key) const noexcept
    {
        return key.hash_value();
    }
};
#endif
