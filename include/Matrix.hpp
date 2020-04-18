#pragma once
#include <algorithm>
#include "Vector.h"

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
		static constexpr const Matrix& Identity() noexcept
		{
			static constexpr auto identity = []()
			{
				Matrix matrix;
				for (size_t i = 0; i < std::min(R, C); ++i)
					matrix[i][i] = 1;
				return matrix;
			}();

			return identity;
		}

		static constexpr const Matrix& Zero() noexcept
		{
			thread_local constexpr Matrix zero;
			return zero;
		}

		template <class... Args>
		constexpr Matrix(Args... args) noexcept
			: arr{static_cast<T>(args)...}
		{
		}

		constexpr bool operator==(const Matrix&) const noexcept = default;

		constexpr auto& operator[](size_t i) noexcept { return m[i]; }
		constexpr auto& operator[](size_t i) const noexcept { return m[i]; }
		constexpr auto& Row(size_t i) noexcept { return m[i]; }
		[[nodiscard]] constexpr auto& Row(size_t i) const noexcept { return m[i]; }

		[[nodiscard]] constexpr auto Col(size_t c) const noexcept
		{
			Vector<T, R> v;
			for (size_t r = 0; r < R; ++r) v[r] = m[r][c];
			return v;
		}

		constexpr Matrix operator+(const Matrix& b) const noexcept
		{
			auto c = *this;
			return c += b;
		}

		constexpr Matrix& operator+=(const Matrix& b) noexcept
		{
			for (auto i = 0; i < R; ++i) m[i] += b[i];
			return *this;
		}

		constexpr Matrix operator-(const Matrix& b) const noexcept
		{
			auto c = *this;
			return c -= b;
		}

		constexpr Matrix& operator-=(const Matrix& b) noexcept
		{
			for (auto i = 0; i < R; ++i) m[i] -= b[i];
			return *this;
		}

		constexpr Matrix operator*(T f) const noexcept
		{
			auto c = *this;
			return c *= f;
		}

		constexpr Matrix& operator*=(T f) noexcept
		{
			for (auto i = 0; i < R; ++i) m[i] *= f;
			return *this;
		}

		constexpr Matrix operator/(T f) const noexcept
		{
			auto c = *this;
			return c /= f;
		}

		constexpr Matrix& operator/=(T f) noexcept
		{
			for (auto i = 0; i < R; ++i) m[i] /= f;
			return *this;
		}

		template <size_t C2>
		constexpr Matrix<T, R, C2> operator*(const Matrix<T, C, C2>& b) const noexcept
		{
			Matrix<T, R, C2> c;
			for (size_t i = 0; i < R; ++i)
				for (size_t j = 0; j < C2; ++j)
					c[i][j] = Row(i) | b.Col(j);
			return c;
		}

		constexpr Matrix& operator*=(const Matrix& b) noexcept { return *this = *this * b; }

		[[nodiscard]] constexpr Matrix<T, C, R> Transpose() const noexcept
		{
			Matrix<T, C, R> t;
			for (auto i = 0; i < R; ++i)
				for (auto j = 0; j < C; ++j)
					t[j][i] = m[i][j];
			return t;
		}

	private:
		union
		{
			Vector<T, C> m[R];
			T arr[R][C];
		};
	};

	template <class F, class T, size_t R, size_t C>
	constexpr auto operator*(F f, const Matrix<T, R, C>& m) noexcept { return m * f; }

	template <class T, size_t R, size_t C>
	std::ostream& operator<<(std::ostream& os, const Matrix<T, R, C>& m)
	{
		os << m[0];
		for (size_t i = 1; i < R; ++i)
			os << '\n' << m[i];
		return os;
	}
}
