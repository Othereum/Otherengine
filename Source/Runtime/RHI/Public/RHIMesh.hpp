#pragma once

namespace oeng
{
inline namespace rhi
{
class RHIMesh
{
public:
    INTERFACE_BODY(RHIMesh);
    virtual void Activate() const noexcept = 0;
};
}
}
