#pragma once
#include <optional>
#include "Path.hpp"
#include "Math.hpp"

namespace oeng
{
	class ISprite
	{
	public:
		struct DrawInfo { Mat4 transform; };
	
		[[nodiscard]] virtual std::optional<DrawInfo> Draw() const noexcept = 0;
		[[nodiscard]] virtual int GetDrawOrder() const noexcept = 0;

		constexpr ISprite() noexcept = default;
		virtual ~ISprite() = default;
		
		ISprite(const ISprite&) = delete;
		ISprite(ISprite&&) = delete;
		ISprite& operator=(const ISprite&) = delete;
		ISprite& operator=(ISprite&&) = delete;
	};

	class IMesh
	{
	public:
		struct DrawInfo { Mat4 transform; size_t vertices; };
	
		[[nodiscard]] virtual std::optional<DrawInfo> Draw() const noexcept = 0;
		[[nodiscard]] virtual Path GetShaderPath() const noexcept = 0;

		constexpr IMesh() noexcept = default;
		virtual ~IMesh() = default;
		
		IMesh(const IMesh&) = delete;
		IMesh(IMesh&&) = delete;
		IMesh& operator=(const IMesh&) = delete;
		IMesh& operator=(IMesh&&) = delete;
	};
}
