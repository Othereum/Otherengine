#include "RHIShader.hpp"

namespace oeng
{
inline namespace rhi
{
void RHIShader::UpdateCache(Name name, Float value)
{
    scalar_cache_.insert_or_assign(name, value);
}

void RHIShader::UpdateCache(Name name, const Vec4& value)
{
    vector_cache_.insert_or_assign(name, value);
}

void RHIShader::UpdateCache(Name name, const Mat4& value)
{
    matrix_cache_.insert_or_assign(name, value);
}

void RHIShader::UpdateCache(Name name, RHITexture& value)
{
    texture_cache_.insert_or_assign(name, value);
}

bool RHIShader::IsRedundant(Name name, Float value) const noexcept
{
    if (const auto it = scalar_cache_.find(name); it != scalar_cache_.end())
        return IsNearlyEqual(value, it->second);

    return false;
}

bool RHIShader::IsRedundant(Name name, const Vec4& value) const noexcept
{
    if (const auto it = vector_cache_.find(name); it != vector_cache_.end())
        return IsNearlyEqual(value, it->second);

    return false;
}

bool RHIShader::IsRedundant(Name name, const Mat4& value) const noexcept
{
    if (const auto it = matrix_cache_.find(name); it != matrix_cache_.end())
        return IsNearlyEqual(value, it->second);

    return false;
}

bool RHIShader::IsRedundant(Name name, RHITexture& value) const noexcept
{
    if (const auto it = texture_cache_.find(name); it != texture_cache_.end())
        return &value == &it->second.get();

    return false;
}
} // namespace rhi
} // namespace oeng
