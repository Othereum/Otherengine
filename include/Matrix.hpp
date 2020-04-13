#pragma once

namespace game
{
	struct Mat4
	{
		static const Mat4 identity;
		
		template <class... Args>
		constexpr Mat4(Args... args) noexcept
			:m{float(args)...}
		{
		}
		
		constexpr bool operator==(const Mat4&) const noexcept = default;
		
		constexpr auto& operator[](size_t i) noexcept { return m[i]; }
		constexpr auto& operator[](size_t i) const noexcept { return m[i]; }

		constexpr Mat4 operator+(const Mat4& b) const noexcept { auto c = *this; return c += b; }
		constexpr Mat4& operator+=(const Mat4& b) & noexcept
		{
			for (auto i=0; i<16; ++i) flat[i] += b.flat[i];
			return *this;
		}

		Mat4 operator*(const Mat4&) const noexcept;
		Mat4& operator*=(const Mat4& b) & noexcept { return *this = *this * b; }

	private:
		union
		{
			float m[4][4];
			float flat[16];
		};
	};
}
