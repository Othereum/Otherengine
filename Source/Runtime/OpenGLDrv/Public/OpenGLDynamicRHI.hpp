#pragma once
#include "DynamicRHI.hpp"

namespace oeng
{
inline namespace opengldrv
{
class OPENGLDRV_API OpenGLDynamicRHI : public DynamicRHI
{
public:
    [[nodiscard]] SharedRef<Texture> CreateTexture(const fs::path& filepath) override;
};
}
}
