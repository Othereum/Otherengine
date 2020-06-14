#pragma once
#include "MathFwd.hpp"

namespace oeng
{
	class Mesh;
	class Material;
	class Texture;

	class IDrawable
	{
	public:
		[[nodiscard]] virtual bool ShouldDraw() const noexcept = 0;
		[[nodiscard]] virtual const Mat4& GetDrawTrsf() const noexcept = 0;

		constexpr IDrawable() noexcept = default;
		virtual ~IDrawable() = default;
		
		IDrawable(const IDrawable&) = delete;
		IDrawable(IDrawable&&) = delete;
		IDrawable& operator=(const IDrawable&) = delete;
		IDrawable& operator=(IDrawable&&) = delete;
	};
	
	class ISpriteComponent : public IDrawable
	{
	public:
		[[nodiscard]] virtual int GetDrawOrder() const noexcept = 0;
		[[nodiscard]] virtual Texture& GetTexture() const noexcept = 0;
	};

	class IMeshComponent : public IDrawable
	{
	public:
		[[nodiscard]] virtual Material& GetMaterial() const noexcept = 0;
		[[nodiscard]] virtual Mesh& GetMesh() const noexcept = 0;
	};
}
