#pragma once

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
    explicit ShaderCompileError(const std::string& message)
        : std::runtime_error{message}
    {
    }

    explicit ShaderCompileError(const char* message)
        : std::runtime_error{message}
    {
    }
};

class RHIShader
{
public:
    INTERFACE_BODY(RHIShader);

    virtual void Activate() const noexcept = 0;

    virtual bool ApplyParam(Name name, Float value) = 0;
    virtual bool ApplyParam(Name name, const Vec4& value) = 0;
    virtual bool ApplyParam(Name name, const RHITexture& value) = 0;

    [[nodiscard]] virtual bool IsScalarParam(Name name) const = 0;
    [[nodiscard]] virtual bool IsVectorParam(Name name) const = 0;
    [[nodiscard]] virtual bool IsTextureParam(Name name) const = 0;

protected:
    void UpdateCache(Name name, Float value);
    void UpdateCache(Name name, const Vec4& value);
    [[nodiscard]] bool IsRedundant(Name name, Float value) const noexcept;
    [[nodiscard]] bool IsRedundant(Name name, const Vec4& value) const noexcept;

private:
    std::unordered_map<Name, Float> scalar_cache_;
    std::unordered_map<Name, Vec4> vector_cache_;
};
}
}
