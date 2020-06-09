#pragma once
#include "Math.hpp"

namespace oeng
{
	class ICamera
	{
	public:
		[[nodiscard]] virtual Mat4 GetViewProj(Vec2u16 scr) const noexcept = 0;
		
		ICamera(const ICamera&) = delete;
		ICamera(ICamera&&) = delete;
		ICamera& operator=(const ICamera&) = delete;
		ICamera& operator=(ICamera&&) = delete;

	protected:
		constexpr ICamera() noexcept = default;
		~ICamera() = default;
	};
}