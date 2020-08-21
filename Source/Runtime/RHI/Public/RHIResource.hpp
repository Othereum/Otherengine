#pragma once

namespace oeng
{
inline namespace rhi
{
class RHIResource
{
public:
    INTERFACE_BODY(RHIResource);
    virtual void Activate() const noexcept = 0;
};
}
}
