#pragma once

namespace oeng
{
inline namespace rhi
{

class RHITexture
{
  public:
    virtual ~RHITexture() = default;
    virtual void Activate() const noexcept = 0;
};

} // namespace rhi
} // namespace oeng
