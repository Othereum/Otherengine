#include "IRenderer.hpp"

namespace oeng
{
inline namespace engine
{
OE_EXPORT IRenderer* renderer;

IRenderer& IRenderer::Get() noexcept
{
    assert(renderer);
    return *renderer;
}
}
}
