#pragma once

namespace oeng
{
inline namespace rhi
{
class RHIMesh
{
INTERFACE_BODY(RHIMesh)

public:
    virtual void Activate() const noexcept = 0;
};
}
}
