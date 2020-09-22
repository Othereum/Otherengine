#pragma once
#include "ShaderParam.hpp"

namespace oeng
{
inline namespace rhi
{
class RHITexture;

/**
 * Exception class indicating shader compilation error.
 * `what()` returns the compilation error log.
 */
class ShaderCompileError : public std::runtime_error
{
  public:
    explicit ShaderCompileError(const std::string& message) : std::runtime_error{message}
    {
    }

    explicit ShaderCompileError(const char* message) : std::runtime_error{message}
    {
    }
};

class RHI_API RHIShader
{
  public:
    virtual ~RHIShader() = default;
    virtual void Activate() const noexcept = 0;

    /**
     * Apply shader parameter. Shader should be activated.
     * @param name Parameter name.
     * @param value New value to be set.
     * @return `true` if successful.
     */
    virtual bool ApplyParam(Name name, ScalarParam value) = 0;
    virtual bool ApplyParam(Name name, const VectorParam& value) = 0;
    virtual bool ApplyParam(Name name, const MatrixParam& value) = 0;
    virtual bool ApplyParam(Name name, RHITexture& value) = 0;

    /**
     * Check if parameter name is valid.
     * @param name Parameter name to test.
     * @return `true` if valid.
     */
    [[nodiscard]] virtual bool IsScalarParam(Name name) const = 0;
    [[nodiscard]] virtual bool IsVectorParam(Name name) const = 0;
    [[nodiscard]] virtual bool IsMatrixParam(Name name) const = 0;
    [[nodiscard]] virtual bool IsTextureParam(Name name) const = 0;

  protected:
    void UpdateCache(Name name, ScalarParam value);
    void UpdateCache(Name name, const VectorParam& value);
    void UpdateCache(Name name, const MatrixParam& value);
    void UpdateCache(Name name, RHITexture& value);
    [[nodiscard]] bool IsRedundant(Name name, ScalarParam value) const noexcept;
    [[nodiscard]] bool IsRedundant(Name name, const VectorParam& value) const noexcept;
    [[nodiscard]] bool IsRedundant(Name name, const MatrixParam& value) const noexcept;
    [[nodiscard]] bool IsRedundant(Name name, RHITexture& value) const noexcept;

  private:
    std::unordered_map<Name, ScalarParam> scalar_cache_;
    std::unordered_map<Name, VectorParam> vector_cache_;
    std::unordered_map<Name, MatrixParam> matrix_cache_;
    std::unordered_map<Name, std::reference_wrapper<RHITexture>> texture_cache_;
};
} // namespace rhi
} // namespace oeng
