#pragma once
#include <algorithm>

namespace game
{
	template <class T, size_t R, size_t C = R>
	struct Matrix;

	using Mat2 = Matrix<float, 2>;
	using Mat3 = Matrix<float, 3>;
	using Mat4 = Matrix<float, 4>;
	
	template <class T, size_t R, size_t C>
	struct Matrix
	{
		static constexpr const Matrix& Identity()
		{
			static constexpr auto identity = []
			{
				Matrix matrix;
				for (size_t i = 0; i < std::min(R, C); ++i)
					matrix[i][i] = 1;
				return matrix;
			}();
			
			return identity;
		}
		
		static constexpr const Matrix& Zero()
		{
			static constexpr Matrix zero;
			return zero;
		}
		
		template <class... Args>
		constexpr Matrix(Args... args) noexcept
			:m{static_cast<float>(args)...}
		{
		}
		
		constexpr bool operator==(const Matrix&) const noexcept = default;
		
		constexpr auto& operator[](size_t i) noexcept { return m[i]; }
		constexpr auto& operator[](size_t i) const noexcept { return m[i]; }

		constexpr Matrix operator+(const Matrix& b) const noexcept { auto c = *this; return c += b; }
		constexpr Matrix& operator+=(const Matrix& b) & noexcept
		{
			for (auto i=0; i<R*C; ++i) m[0][i] += b.m[0][i];
			return *this;
		}

		constexpr Matrix operator-(const Matrix& b) const noexcept { auto c = *this; return c -= b; }
		constexpr Matrix& operator-=(const Matrix& b) & noexcept
		{
			for (auto i=0; i<R*C; ++i) m[0][i] -= b.m[0][i];
			return *this;
		}

		constexpr Matrix operator*(float f) const noexcept { auto c = *this; return c *= f; }
		constexpr Matrix& operator*=(float f) & noexcept
		{
			for (auto i=0; i<R*C; ++i) m[0][i] *= f;
			return *this;
		}

		constexpr Matrix operator*(const Matrix& b) const noexcept
		{
			/*
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
			*/
		}
		constexpr Matrix& operator*=(const Matrix& b) & noexcept { return *this = *this * b; }

		constexpr std::enable_if_t<R==C> transpose() noexcept { *this = transposed(); }
		[[nodiscard]] constexpr Matrix<T, C, R> transposed() const noexcept
		{
			Matrix<T, C, R> t;
			for (auto i=0; i<R; ++i) for (auto j=0; j<C; ++j) t[j][i] = m[i][j];
			return t;
		}

	private:
		T m[R][C];
	};
}
