#pragma once

namespace oeng
{
inline namespace rhi
{
class RHIMesh
{
public:
    virtual ~RHIMesh() = default;
    virtual void Activate() const noexcept = 0;
};
}
}
