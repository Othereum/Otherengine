#include "Interfaces/Drawable.hpp"
#include "Math.hpp"
#include "Mesh.hpp"

namespace oeng
{
	Float IMeshComponent::GetRadius() const noexcept
	{
		return Max(GetDrawTrsf().scale) * GetMesh().GetRadius();
	}
}
