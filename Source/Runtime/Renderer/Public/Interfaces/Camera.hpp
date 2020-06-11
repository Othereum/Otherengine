#pragma once
#include "Math.hpp"

namespace oeng
{
	class ICamera
	{
	public:
		[[nodiscard]] virtual const Vec3& GetPos() const noexcept = 0;
		[[nodiscard]] virtual const Mat4& GetViewProj() const noexcept = 0;
		virtual void OnScreenSizeChanged(Vec2u16 scr) = 0;
		
		constexpr ICamera() noexcept = default;
		virtual ~ICamera() = default;
		
		ICamera(const ICamera&) = delete;
		ICamera(ICamera&&) = delete;
		ICamera& operator=(const ICamera&) = delete;
		ICamera& operator=(ICamera&&) = delete;
	};
}