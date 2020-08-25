#pragma once
#include "RHIResource.hpp"
#include "ShaderParam.hpp"

namespace oeng
{
inline namespace rhi
{
/**
 * Exception class indicating shader compilation error.
 * `what()` returns the compilation error log.
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

class RHIShader : public RHIResource
{
public:
    /**
     * Set parameter value of this shader with given name.
     * @param name The name of the parameter.
     * @param value The new value to be set.
     * @return `true` if successful.
     */
    virtual bool SetParam(Name name, const ShaderParam& value) = 0;

    /**
     * Check if the parameter name is valid.
     * @param name Parameter name to check.
     * @return `true` if valid.
     */
    [[nodiscard]] virtual bool IsValidParam(Name name) const noexcept = 0;

protected:
    void UpdateCache(Name name, const ShaderParam& value);
    [[nodiscard]] bool IsRedundant(Name name, const ShaderParam& value) const noexcept;

private:
    std::unordered_map<Name, ShaderParam> param_cache_;
};
}
}
