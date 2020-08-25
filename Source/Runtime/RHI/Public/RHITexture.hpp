#pragma once

namespace oeng
{
inline namespace rhi
{
class RHITexture
{
public:
    INTERFACE_BODY(RHITexture);
    virtual void Activate() const noexcept = 0;
};
}
}
