#pragma once

namespace game
{
	struct Mat4
	{
		template <class... Args>
		constexpr Mat4(Args... args) noexcept
			:m{float(args)...}
		{
		}
		
		constexpr auto& operator[](size_t i) noexcept { return m[i]; }
		constexpr auto& operator[](size_t i) const noexcept { return m[i]; }

		Mat4 operator*(const Mat4&) const noexcept;

	private:
		float m[4][4];
	};
}
