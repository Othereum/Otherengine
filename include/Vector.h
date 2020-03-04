#pragma once

namespace game
{
	template <class T>
	struct vector2;

	using fvector2 = vector2<float>;
	
	template <class T>
	struct vector2
	{
		T x{}, y{};

		constexpr vector2() noexcept = default;
		constexpr vector2(T x, T y) noexcept :x{x}, y{y} {}

		template <class U>
		constexpr explicit vector2(const vector2<U>& v) noexcept :x{T(v.x)}, y{T(v.y)} {}

		[[nodiscard]] constexpr auto lensqr() const noexcept { return x*x + y*y; }
		[[nodiscard]] float len() const noexcept { return sqrtf(lensqr()); }

		constexpr vector2& operator+=(const vector2& a)& noexcept
		{
			return *this = *this + a;
		}

		constexpr vector2& operator-=(const vector2& a)& noexcept
		{
			return *this = *this - a;
		}

		constexpr vector2& operator*=(T f)& noexcept
		{
			return *this = *this * f;
		}

		constexpr vector2& operator/=(T f)& noexcept
		{
			return *this = *this / f;
		}

		template <class U>
		constexpr vector2<std::common_type_t<T, U>> operator+(const vector2<U>& v) const noexcept
		{
			return {x + v.x, y + v.y};
		}

		template <class U>
		constexpr vector2<std::common_type_t<T, U>> operator-(const vector2<U>& v) const noexcept
		{
			return {x - v.x, y - v.y};
		}

		template <class U>
		constexpr vector2<std::common_type_t<T, U>> operator*(U f) const noexcept
		{
			return {x*f, y*f};
		}

		template <class U>
		constexpr vector2<std::common_type_t<T, U>> operator/(U f) const noexcept
		{
			return {x/f, y/f};
		}
	};

	template <class T, class U>
	constexpr vector2<std::common_type_t<T, U>> operator*(U f, const vector2<T>& v) noexcept
	{
		return v * f;
	}
}
