#pragma once
#include "Math.hpp"
#include "Interfaces/Base.hpp"

namespace oeng
{
	class ICamera
	{
		OE_INTERFACE_BODY(ICamera);
		
		[[nodiscard]] virtual const Vec3& GetPos() const noexcept = 0;
		[[nodiscard]] virtual const Mat4& GetViewProj() const noexcept = 0;
		virtual void OnScreenSizeChanged(Vec2u16 scr) = 0;
	};
}
