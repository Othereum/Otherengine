#pragma once
#include "Core.hpp"
#include "MathFwd.hpp"

namespace oeng
{
	class Mesh;
	class Material;
	class Texture;

	class OEAPI ISpriteComponent
	{
	public:
		INTERFACE_BODY(ISpriteComponent);
		
		[[nodiscard]] virtual bool ShouldDraw() const noexcept = 0;
		[[nodiscard]] virtual const Mat4& GetDrawTrsf() const noexcept = 0;
		[[nodiscard]] virtual int GetDrawOrder() const noexcept = 0;
		[[nodiscard]] virtual Texture& GetTexture() const noexcept = 0;
	};

	class OEAPI IMeshComponent
	{
	public:
		INTERFACE_BODY(IMeshComponent);

		[[nodiscard]] virtual Float GetMaxDrawDist() const noexcept = 0;
		[[nodiscard]] virtual Float GetRadius() const noexcept = 0;
		[[nodiscard]] virtual bool ShouldDraw() const noexcept = 0;
		[[nodiscard]] virtual const Transform& GetDrawTrsf() const noexcept = 0;
		[[nodiscard]] virtual const Mat4& GetDrawTrsfMatrix() const noexcept = 0;
		[[nodiscard]] virtual Material& GetMaterial() const noexcept = 0;
		[[nodiscard]] virtual Mesh& GetMesh() const noexcept = 0;
	};
}
