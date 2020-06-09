#pragma once
#include "Path.hpp"
#include "Math.hpp"

namespace oeng
{
	class ISprite
	{
	public:
		struct DrawInfo { Mat4 transform; };
	
		[[nodiscard]] virtual DrawInfo Draw() const noexcept = 0;
		[[nodiscard]] virtual int GetDrawOrder() const noexcept = 0;

		ISprite(const ISprite&) = delete;
		ISprite(ISprite&&) = delete;
		ISprite& operator=(const ISprite&) = delete;
		ISprite& operator=(ISprite&&) = delete;

	protected:
		constexpr ISprite() noexcept = default;
		~ISprite() = default;
	};

	class IMesh
	{
	public:
		struct DrawInfo { Mat4 transform; int vertices; };
	
		[[nodiscard]] virtual DrawInfo Draw() const noexcept = 0;
		[[nodiscard]] virtual Path GetShaderPath() const noexcept = 0;

		IMesh(const IMesh&) = delete;
		IMesh(IMesh&&) = delete;
		IMesh& operator=(const IMesh&) = delete;
		IMesh& operator=(IMesh&&) = delete;

	protected:
		constexpr IMesh() noexcept = default;
		~IMesh() = default;
	};
}
