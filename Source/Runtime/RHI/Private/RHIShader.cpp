#include "RHIShader.hpp"

namespace oeng
{
inline namespace rhi
{
void RHIShader::UpdateCache(Name name, ScalarParam value)
{
    scalar_cache_.insert_or_assign(name, value);
}

void RHIShader::UpdateCache(Name name, const VectorParam& value)
{
    vector_cache_.insert_or_assign(name, value);
}

void RHIShader::UpdateCache(Name name, const MatrixParam& value)
{
    matrix_cache_.insert_or_assign(name, value);
}

void RHIShader::UpdateCache(Name name, RHITexture& value)
{
    texture_cache_.insert_or_assign(name, value);
}

static constexpr auto is_equivalent = []<class T1, class T2>(const T1& a, const T2& b) {
    if constexpr (std::is_same_v<T1, T2>)
        return IsNearlyEqual(a, b);
    else
        return false;
};

bool RHIShader::IsRedundant(Name name, ScalarParam value) const noexcept
{
    if (const auto it = scalar_cache_.find(name); it != scalar_cache_.end())
        return std::visit(is_equivalent, value, it->second);

    return false;
}

bool RHIShader::IsRedundant(Name name, const VectorParam& value) const noexcept
{
    if (const auto it = vector_cache_.find(name); it != vector_cache_.end())
        return std::visit(is_equivalent, value, it->second);

    return false;
}

bool RHIShader::IsRedundant(Name name, const MatrixParam& value) const noexcept
{
    if (const auto it = matrix_cache_.find(name); it != matrix_cache_.end())
        return std::visit(is_equivalent, value, it->second);

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
