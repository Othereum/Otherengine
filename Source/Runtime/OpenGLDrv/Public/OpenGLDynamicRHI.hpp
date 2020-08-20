#pragma once
#include "DynamicRHI.hpp"

namespace oeng
{
inline namespace opengldrv
{
class OPENGLDRV_API OpenGLDynamicRHI : public DynamicRHI
{
public:
    [[nodiscard]] RHITexture* CreateTexture(const TextureData& data) const override;
};
}
}
