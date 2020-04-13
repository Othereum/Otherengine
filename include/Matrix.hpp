#pragma once

namespace game
{
	struct Mat4
	{
		static constexpr Mat4 identity()
		{
			return {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}
		
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

		constexpr Mat4 operator*(float f) const noexcept { auto c = *this; return c *= f; }
		constexpr Mat4& operator*=(float f) & noexcept
		{
			for (auto& v : flat) v *= f;
			return *this;
		}

		constexpr Mat4 operator*(const Mat4& b) const noexcept
		{
			auto& a = *this;
			return {
				a[0][0]*b[0][0]+a[0][1]*b[1][0]+a[0][2]*b[2][0]+a[0][3]*b[3][0], a[0][0]*b[0][1]+a[0][1]*b[1][1]+a[0][2]*b[2][1]+a[0][3]*b[3][1],
				a[0][0]*b[0][2]+a[0][1]*b[1][2]+a[0][2]*b[2][2]+a[0][3]*b[3][2], a[0][0]*b[0][3]+a[0][1]*b[1][3]+a[0][2]*b[2][3]+a[0][3]*b[3][3],
				
				a[1][0]*b[0][0]+a[1][1]*b[1][0]+a[1][2]*b[2][0]+a[1][3]*b[3][0], a[1][0]*b[0][1]+a[1][1]*b[1][1]+a[1][2]*b[2][1]+a[1][3]*b[3][1],
				a[1][0]*b[0][2]+a[1][1]*b[1][2]+a[1][2]*b[2][2]+a[1][3]*b[3][2], a[1][0]*b[0][3]+a[1][1]*b[1][3]+a[1][2]*b[2][3]+a[1][3]*b[3][3],
				
				a[2][0]*b[0][0]+a[2][1]*b[1][0]+a[2][2]*b[2][0]+a[2][3]*b[3][0], a[2][0]*b[0][1]+a[2][1]*b[1][1]+a[2][2]*b[2][1]+a[2][3]*b[3][1],
				a[2][0]*b[0][2]+a[2][1]*b[1][2]+a[2][2]*b[2][2]+a[2][3]*b[3][2], a[2][0]*b[0][3]+a[2][1]*b[1][3]+a[2][2]*b[2][3]+a[2][3]*b[3][3],
				
				a[3][0]*b[0][0]+a[3][1]*b[1][0]+a[3][2]*b[2][0]+a[3][3]*b[3][0], a[3][0]*b[0][1]+a[3][1]*b[1][1]+a[3][2]*b[2][1]+a[3][3]*b[3][1],
				a[3][0]*b[0][2]+a[3][1]*b[1][2]+a[3][2]*b[2][2]+a[3][3]*b[3][2], a[3][0]*b[0][3]+a[3][1]*b[1][3]+a[3][2]*b[2][3]+a[3][3]*b[3][3]
			};
		}
		constexpr Mat4& operator*=(const Mat4& b) & noexcept { return *this = *this * b; }

	private:
		union
		{
			float m[4][4];
			float flat[16];
		};
	};
}
