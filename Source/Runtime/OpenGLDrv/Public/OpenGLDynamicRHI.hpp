#pragma once
#include "DynamicRHI.hpp"

namespace oeng
{
inline namespace opengldrv
{
class OPENGLDRV_API OpenGLDynamicRHI : public DynamicRHI
{
public:
    RHIResource* CreateTexture(Vec2i size, int channels, const void* pixels) const override;
};
}
}
