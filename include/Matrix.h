#pragma once

namespace game
{
	struct Mat4
	{
		template <class... Args>
		explicit constexpr Mat4(Args&&... args) noexcept
			:m{args...}
		{
		}
		
		auto& operator[](size_t i) noexcept { return m[i]; }
		auto& operator[](size_t i) const noexcept { return m[i]; }

		Mat4 operator*(const Mat4& o) const noexcept
		{
		}
		
	private:
		float m[4][4];
	};
}
