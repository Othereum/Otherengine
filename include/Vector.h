#pragma once
#include <type_traits>

namespace game
{
	template <class T>
	struct Vector2;

	using FVector2 = Vector2<float>;
	
	template <class T>
	struct Vector2
	{
		T x{}, y{};

		constexpr Vector2() noexcept = default;
		constexpr Vector2(T x, T y) noexcept :x{x}, y{y} {}

		template <class U>
		constexpr explicit Vector2(const Vector2<U>& v) noexcept :x{T(v.x)}, y{T(v.y)} {}

		[[nodiscard]] constexpr auto LenSqr() const noexcept { return x*x + y*y; }
		[[nodiscard]] float Len() const noexcept { return sqrtf(LenSqr()); }

		template <class U>
		[[nodiscard]] float Dist(const Vector2<U>& v) const noexcept { return (*this - v).Len(); }

		template <class U>
		[[nodiscard]] float DistSqr(const Vector2<U>& v) const noexcept { return (*this - v).LenSqr(); }

		void Normalize() noexcept { *this /= Len(); }
		[[nodiscard]] Vector2 Normal() const noexcept { auto x = *this; x.Normalize(); return x; }

		constexpr Vector2& operator+=(const Vector2& a)& noexcept
		{
			return *this = *this + a;
		}

		constexpr Vector2& operator-=(const Vector2& a)& noexcept
		{
			return *this = *this - a;
		}

		constexpr Vector2& operator*=(T f)& noexcept
		{
			return *this = *this * f;
		}

		constexpr Vector2& operator/=(T f)& noexcept
		{
			return *this = *this / f;
		}

		template <class U>
		constexpr Vector2<std::common_type_t<T, U>> operator+(const Vector2<U>& v) const noexcept
		{
			return {x + v.x, y + v.y};
		}

		template <class U>
		constexpr Vector2<std::common_type_t<T, U>> operator-(const Vector2<U>& v) const noexcept
		{
			return {x - v.x, y - v.y};
		}

		template <class U>
		constexpr Vector2<std::common_type_t<T, U>> operator*(U f) const noexcept
		{
			return {x*f, y*f};
		}

		template <class U>
		constexpr Vector2<std::common_type_t<T, U>> operator/(U f) const noexcept
		{
			return {x/f, y/f};
		}

		template <class U>
		constexpr auto operator|(const Vector2<U>& v) const noexcept
		{
			return x*v.x + y*v.y;
		}
	};

	template <class T, class U>
	constexpr Vector2<std::common_type_t<T, U>> operator*(U f, const Vector2<T>& v) noexcept
	{
		return v * f;
	}

	template <class T>
	struct Vector3;

	using FVector3 = Vector3<float>;
	
	template <class T>
	struct Vector3
	{
		T x{}, y{}, z{};

		constexpr Vector3() noexcept = default;
		constexpr Vector3(T x, T y, T z) noexcept :x{x}, y{y}, z{z} {}

		template <class U>
		constexpr explicit Vector3(const Vector3<U>& v) noexcept :x{T(v.x)}, y{T(v.y)}, z{T(v.z)} {}

		[[nodiscard]] constexpr auto LenSqr() const noexcept { return x*x + y*y + z*z; }
		[[nodiscard]] float Len() const noexcept { return sqrtf(LenSqr()); }

		template <class U>
		[[nodiscard]] float Dist(const Vector3<U>& v) const noexcept { return (*this - v).Len(); }

		template <class U>
		[[nodiscard]] float DistSqr(const Vector3<U>& v) const noexcept { return (*this - v).LenSqr(); }

		void Normalize() noexcept { *this /= Len(); }
		[[nodiscard]] Vector3 Normal() const noexcept { auto x = *this; x.Normalize(); return x; }

		constexpr Vector3& operator+=(const Vector3& a)& noexcept
		{
			return *this = *this + a;
		}

		constexpr Vector3& operator-=(const Vector3& a)& noexcept
		{
			return *this = *this - a;
		}

		constexpr Vector3& operator*=(T f)& noexcept
		{
			return *this = *this * f;
		}

		constexpr Vector3& operator/=(T f)& noexcept
		{
			return *this = *this / f;
		}

		template <class U>
		constexpr Vector3<std::common_type_t<T, U>> operator+(const Vector3<U>& v) const noexcept
		{
			return {x + v.x, y + v.y, z + v.z};
		}

		template <class U>
		constexpr Vector3<std::common_type_t<T, U>> operator-(const Vector3<U>& v) const noexcept
		{
			return {x - v.x, y - v.y, z - v.z};
		}

		template <class U>
		constexpr Vector3<std::common_type_t<T, U>> operator*(U f) const noexcept
		{
			return {x*f, y*f, z*f};
		}

		template <class U>
		constexpr Vector3<std::common_type_t<T, U>> operator/(U f) const noexcept
		{
			return {x/f, y/f, z/f};
		}

		template <class U>
		constexpr auto operator|(const Vector3<U>& v) const noexcept
		{
			return x*v.x + y*v.y + z*v.z;
		}

		template <class U>
		constexpr Vector3<std::common_type_t<T, U>> operator^(const Vector3<U>& v) const noexcept
		{
			return {y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x};
		}
	};

	template <class T, class U>
	constexpr Vector3<std::common_type_t<T, U>> operator*(U f, const Vector3<T>& v) noexcept
	{
		return v * f;
	}
}
