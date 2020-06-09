#pragma once
#include "Math.hpp"

namespace oeng
{
	class ICamera
	{
	public:
		[[nodiscard]] virtual Mat4 GetViewProj() = 0;
		virtual void OnScreenSizeChanged(Vec2u16 scr) = 0;
		
		constexpr ICamera() noexcept = default;
		virtual ~ICamera() = default;
		
		ICamera(const ICamera&) = delete;
		ICamera(ICamera&&) = delete;
		ICamera& operator=(const ICamera&) = delete;
		ICamera& operator=(ICamera&&) = delete;
	};
}