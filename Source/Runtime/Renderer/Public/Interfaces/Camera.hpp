#pragma once
#include "Math.hpp"

namespace oeng
{
	class ICamera
	{
	public:
		[[nodiscard]] virtual Mat4 GetViewProj(Vec2u16 scr) const noexcept = 0;
		
		constexpr ICamera() noexcept = default;
		virtual ~ICamera() = default;
		
		ICamera(const ICamera&) = delete;
		ICamera(ICamera&&) = delete;
		ICamera& operator=(const ICamera&) = delete;
		ICamera& operator=(ICamera&&) = delete;
	};
}