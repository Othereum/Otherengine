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
			for (auto i=0; i<16; ++i) m[0][i] += b.m[0][i];
			return *this;
		}

		constexpr Mat4 operator-(const Mat4& b) const noexcept { auto c = *this; return c -= b; }
		constexpr Mat4& operator-=(const Mat4& b) & noexcept
		{
			for (auto i=0; i<16; ++i) m[0][i] -= b.m[0][i];
			return *this;
		}

		constexpr Mat4 operator*(float f) const noexcept { auto c = *this; return c *= f; }
		constexpr Mat4& operator*=(float f) & noexcept
		{
			for (auto i=0; i<16; ++i) m[0][i] *= f;
			return *this;
		}

		constexpr Mat4 operator*(const Mat4& b) const noexcept
		{
			return {
				m[0][0]*b[0][0]+m[0][1]*b[1][0]+m[0][2]*b[2][0]+m[0][3]*b[3][0], m[0][0]*b[0][1]+m[0][1]*b[1][1]+m[0][2]*b[2][1]+m[0][3]*b[3][1],
				m[0][0]*b[0][2]+m[0][1]*b[1][2]+m[0][2]*b[2][2]+m[0][3]*b[3][2], m[0][0]*b[0][3]+m[0][1]*b[1][3]+m[0][2]*b[2][3]+m[0][3]*b[3][3],
				
				m[1][0]*b[0][0]+m[1][1]*b[1][0]+m[1][2]*b[2][0]+m[1][3]*b[3][0], m[1][0]*b[0][1]+m[1][1]*b[1][1]+m[1][2]*b[2][1]+m[1][3]*b[3][1],
				m[1][0]*b[0][2]+m[1][1]*b[1][2]+m[1][2]*b[2][2]+m[1][3]*b[3][2], m[1][0]*b[0][3]+m[1][1]*b[1][3]+m[1][2]*b[2][3]+m[1][3]*b[3][3],
				
				m[2][0]*b[0][0]+m[2][1]*b[1][0]+m[2][2]*b[2][0]+m[2][3]*b[3][0], m[2][0]*b[0][1]+m[2][1]*b[1][1]+m[2][2]*b[2][1]+m[2][3]*b[3][1],
				m[2][0]*b[0][2]+m[2][1]*b[1][2]+m[2][2]*b[2][2]+m[2][3]*b[3][2], m[2][0]*b[0][3]+m[2][1]*b[1][3]+m[2][2]*b[2][3]+m[2][3]*b[3][3],
				
				m[3][0]*b[0][0]+m[3][1]*b[1][0]+m[3][2]*b[2][0]+m[3][3]*b[3][0], m[3][0]*b[0][1]+m[3][1]*b[1][1]+m[3][2]*b[2][1]+m[3][3]*b[3][1],
				m[3][0]*b[0][2]+m[3][1]*b[1][2]+m[3][2]*b[2][2]+m[3][3]*b[3][2], m[3][0]*b[0][3]+m[3][1]*b[1][3]+m[3][2]*b[2][3]+m[3][3]*b[3][3]
			};
		}
		constexpr Mat4& operator*=(const Mat4& b) & noexcept { return *this = *this * b; }

		constexpr void transpose() noexcept { *this = transposed(); }
		[[nodiscard]] constexpr Mat4 transposed() const noexcept
		{
			Mat4 t;
			for (auto i=0; i<4; ++i) for (auto j=0; j<4; ++j) t[i][j] = m[j][i];
			return t;
		}

	private:
		float m[4][4];
	};
}
