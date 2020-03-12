#pragma once
#include <type_traits>

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

		template <class U>
		[[nodiscard]] float dist(const vector2<U>& v) const noexcept { return (*this - v).len(); }

		template <class U>
		[[nodiscard]] float distsqr(const vector2<U>& v) const noexcept { return (*this - v).lensqr(); }

		void normalize() noexcept { *this /= len(); }
		[[nodiscard]] vector2 normal() const noexcept { auto x = *this; x.normalize(); return x; }

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

		template <class U>
		constexpr auto operator|(const vector2<U>& v) const noexcept
		{
			return x*v.x + y*v.y;
		}
	};

	template <class T, class U>
	constexpr vector2<std::common_type_t<T, U>> operator*(U f, const vector2<T>& v) noexcept
	{
		return v * f;
	}

	template <class T>
	struct vector3;

	using fvector3 = vector3<float>;
	
	template <class T>
	struct vector3
	{
		T x{}, y{}, z{};

		constexpr vector3() noexcept = default;
		constexpr vector3(T x, T y, T z) noexcept :x{x}, y{y}, z{z} {}

		template <class U>
		constexpr explicit vector3(const vector3<U>& v) noexcept :x{T(v.x)}, y{T(v.y)}, z{T(v.z)} {}

		[[nodiscard]] constexpr auto lensqr() const noexcept { return x*x + y*y + z*z; }
		[[nodiscard]] float len() const noexcept { return sqrtf(lensqr()); }

		template <class U>
		[[nodiscard]] float dist(const vector3<U>& v) const noexcept { return (*this - v).len(); }

		template <class U>
		[[nodiscard]] float distsqr(const vector3<U>& v) const noexcept { return (*this - v).lensqr(); }

		void normalize() noexcept { *this /= len(); }
		[[nodiscard]] vector3 normal() const noexcept { auto x = *this; x.normalize(); return x; }

		constexpr vector3& operator+=(const vector3& a)& noexcept
		{
			return *this = *this + a;
		}

		constexpr vector3& operator-=(const vector3& a)& noexcept
		{
			return *this = *this - a;
		}

		constexpr vector3& operator*=(T f)& noexcept
		{
			return *this = *this * f;
		}

		constexpr vector3& operator/=(T f)& noexcept
		{
			return *this = *this / f;
		}

		template <class U>
		constexpr vector3<std::common_type_t<T, U>> operator+(const vector3<U>& v) const noexcept
		{
			return {x + v.x, y + v.y, z + v.z};
		}

		template <class U>
		constexpr vector3<std::common_type_t<T, U>> operator-(const vector3<U>& v) const noexcept
		{
			return {x - v.x, y - v.y, z - v.z};
		}

		template <class U>
		constexpr vector3<std::common_type_t<T, U>> operator*(U f) const noexcept
		{
			return {x*f, y*f, z*f};
		}

		template <class U>
		constexpr vector3<std::common_type_t<T, U>> operator/(U f) const noexcept
		{
			return {x/f, y/f, z/f};
		}

		template <class U>
		constexpr auto operator|(const vector3<U>& v) const noexcept
		{
			return x*v.x + y*v.y + z*v.z;
		}

		template <class U>
		constexpr vector3<std::common_type_t<T, U>> operator^(const vector3<U>& v) const noexcept
		{
			return {y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x};
		}
	};

	template <class T, class U>
	constexpr vector3<std::common_type_t<T, U>> operator*(U f, const vector3<T>& v) noexcept
	{
		return v * f;
	}
}
