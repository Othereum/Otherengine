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
		~vector2() noexcept = default;

		template <class U>
		constexpr explicit vector2(const vector2<U>& v) noexcept {*this=v;}
		constexpr vector2(const vector2&) noexcept = default;

		template <class U>
		constexpr explicit vector2(vector2<U>&& v) noexcept :vector2{v} {}
		constexpr vector2(vector2&&) noexcept = default;

		template <class U>
		constexpr vector2& operator=(const vector2<U>& v) noexcept { x=T(v.x); y=T(v.y); return *this; }
		constexpr vector2& operator=(const vector2&) noexcept = default;

		template <class U>
		constexpr vector2& operator=(vector2<U>&& v) noexcept { return *this = v; }
		constexpr vector2& operator=(vector2&&) noexcept = default;

		template <class U>
		constexpr vector2& operator+=(const vector2<U>& a)& noexcept
		{
			x += a.x;
			y += a.y;
			return *this;
		}

		template <class U>
		constexpr vector2<std::common_type_t<T, U>> operator*(U f) const noexcept
		{
			return {x*f, y*f};
		}

		template <class U>
		constexpr vector2<std::common_type_t<T, U>> operator-(const vector2<U>& v) const noexcept
		{
			return {x-v.x, y-v.y};
		}

		template <class U>
		constexpr vector2<std::common_type_t<T, U>> operator/(U f) const noexcept
		{
			return {x/f, y/f};
		}
	};
}
