#pragma once
#include "RHIResource.hpp"
#include <variant>

namespace oeng
{
inline namespace rhi
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

class RHI_API Shader : public RHIResource
{
public:
    using Param = std::variant<
        Float, int32_t,

        Vec2, Vec3, Vec4,
        Vec2i, Vec3i, Vec4i,

        Mat2, Mat3, Mat4,
        Matrix<Float, 2, 3>, Matrix<Float, 2, 4>,
        Matrix<Float, 3, 2>, Matrix<Float, 3, 4>,
        Matrix<Float, 4, 2>, Matrix<Float, 4, 3>
    >;

    /**
     * Set parameter value of this shader with given name.
     * @param name The name of the parameter.
     * @param value The new value to be set.
     * @return True if successful.
     */
    virtual bool SetParam(Name name, const Param& value) = 0;
};
}
}
