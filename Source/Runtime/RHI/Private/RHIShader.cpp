#include "RHIShader.hpp"

namespace oeng
{
inline namespace rhi
{
bool RHIShader::IsRedundant(Name name, const ShaderParam& value) const noexcept
{
    const auto cache = param_cache_.find(name);
    if (cache == param_cache_.end())
        return false;

    auto equals = []<class T1, class T2>(const T1& a, const T2& b)
    {
        if constexpr (!std::is_same_v<T1, T2>)
            return false;

        else
            return IsNearlyEqual(a, b);
    };

    return std::visit(equals, cache->second, value);
}

void RHIShader::UpdateCache(Name name, const ShaderParam& value)
{
    param_cache_.insert_or_assign(name, value);
}
}
}
