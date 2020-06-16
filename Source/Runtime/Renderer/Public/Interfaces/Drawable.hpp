#pragma once
#include "MathFwd.hpp"
#include "Interfaces/Base.hpp"

namespace oeng
{
	class Mesh;
	class Material;
	class Texture;

	class ISpriteComponent
	{
		OE_INTERFACE_BODY(ISpriteComponent);
		
		[[nodiscard]] virtual bool ShouldDraw() const noexcept = 0;
		[[nodiscard]] virtual const Mat4& GetDrawTrsf() const noexcept = 0;
		[[nodiscard]] virtual int GetDrawOrder() const noexcept = 0;
		[[nodiscard]] virtual Texture& GetTexture() const noexcept = 0;
	};

	class IMeshComponent
	{
		OE_INTERFACE_BODY(IMeshComponent);
		
		[[nodiscard]] virtual bool ShouldDraw() const noexcept = 0;
		[[nodiscard]] virtual const Mat4& GetDrawTrsf() const noexcept = 0;
		[[nodiscard]] virtual Material& GetMaterial() const noexcept = 0;
		[[nodiscard]] virtual Mesh& GetMesh() const noexcept = 0;
	};
}
