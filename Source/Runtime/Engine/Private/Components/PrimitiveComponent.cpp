#include "Components/PrimitiveComponent.hpp"
#include "Engine/Engine.hpp"
#include <vector>

namespace oeng
{
inline namespace engine
{
IRenderer& PrimitiveComponent::GetRenderer() const noexcept
{
    return GetEngine().GetRenderer();
}
} // namespace engine
} // namespace oeng
