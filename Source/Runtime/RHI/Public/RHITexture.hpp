#pragma once

namespace oeng
{
inline namespace rhi
{
class RHITexture
{
INTERFACE_BODY(RHITexture)

public:
    virtual void Activate() const noexcept = 0;
};
}
}
