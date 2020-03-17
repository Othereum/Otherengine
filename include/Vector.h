#pragma once
#include <type_traits>

namespace Game
{
	template <class T>
	struct FVector2;

	using TFVector2 = FVector2<float>;
	
	template <class T>
	struct FVector2
	{
		T x{}, y{};

		constexpr FVector2() noexcept = default;
		constexpr FVector2(T x, T y) noexcept :x{x}, y{y} {}

		template <class U>
		constexpr explicit FVector2(const FVector2<U>& v) noexcept :x{T(v.x)}, y{T(v.y)} {}

		[[nodiscard]] constexpr auto LenSqr() const noexcept { return x*x + y*y; }
		[[nodiscard]] float Len() const noexcept { return sqrtf(LenSqr()); }

		template <class U>
		[[nodiscard]] float Dist(const FVector2<U>& v) const noexcept { return (*this - v).Len(); }

		template <class U>
		[[nodiscard]] float DistSqr(const FVector2<U>& v) const noexcept { return (*this - v).LenSqr(); }

		void Normalize() noexcept { *this /= Len(); }
		[[nodiscard]] FVector2 Normal() const noexcept { auto x = *this; x.Normalize(); return x; }

		constexpr FVector2& operator+=(const FVector2& a)& noexcept
		{
			return *this = *this + a;
		}

		constexpr FVector2& operator-=(const FVector2& a)& noexcept
		{
			return *this = *this - a;
		}

		constexpr FVector2& operator*=(T f)& noexcept
		{
			return *this = *this * f;
		}

		constexpr FVector2& operator/=(T f)& noexcept
		{
			return *this = *this / f;
		}

		template <class U>
		constexpr FVector2<std::common_type_t<T, U>> operator+(const FVector2<U>& v) const noexcept
		{
			return {x + v.x, y + v.y};
		}

		template <class U>
		constexpr FVector2<std::common_type_t<T, U>> operator-(const FVector2<U>& v) const noexcept
		{
			return {x - v.x, y - v.y};
		}

		template <class U>
		constexpr FVector2<std::common_type_t<T, U>> operator*(U f) const noexcept
		{
			return {x*f, y*f};
		}

		template <class U>
		constexpr FVector2<std::common_type_t<T, U>> operator/(U f) const noexcept
		{
			return {x/f, y/f};
		}

		template <class U>
		constexpr auto operator|(const FVector2<U>& v) const noexcept
		{
			return x*v.x + y*v.y;
		}
	};

	template <class T, class U>
	constexpr FVector2<std::common_type_t<T, U>> operator*(U f, const FVector2<T>& v) noexcept
	{
		return v * f;
	}

	template <class T>
	struct FVector3;

	using TFVector3 = FVector3<float>;
	
	template <class T>
	struct FVector3
	{
		T x{}, y{}, z{};

		constexpr FVector3() noexcept = default;
		constexpr FVector3(T x, T y, T z) noexcept :x{x}, y{y}, z{z} {}

		template <class U>
		constexpr explicit FVector3(const FVector3<U>& v) noexcept :x{T(v.x)}, y{T(v.y)}, z{T(v.z)} {}

		[[nodiscard]] constexpr auto LenSqr() const noexcept { return x*x + y*y + z*z; }
		[[nodiscard]] float Len() const noexcept { return sqrtf(LenSqr()); }

		template <class U>
		[[nodiscard]] float Dist(const FVector3<U>& v) const noexcept { return (*this - v).Len(); }

		template <class U>
		[[nodiscard]] float DistSqr(const FVector3<U>& v) const noexcept { return (*this - v).LenSqr(); }

		void Normalize() noexcept { *this /= Len(); }
		[[nodiscard]] FVector3 Normal() const noexcept { auto x = *this; x.Normalize(); return x; }

		constexpr FVector3& operator+=(const FVector3& a)& noexcept
		{
			return *this = *this + a;
		}

		constexpr FVector3& operator-=(const FVector3& a)& noexcept
		{
			return *this = *this - a;
		}

		constexpr FVector3& operator*=(T f)& noexcept
		{
			return *this = *this * f;
		}

		constexpr FVector3& operator/=(T f)& noexcept
		{
			return *this = *this / f;
		}

		template <class U>
		constexpr FVector3<std::common_type_t<T, U>> operator+(const FVector3<U>& v) const noexcept
		{
			return {x + v.x, y + v.y, z + v.z};
		}

		template <class U>
		constexpr FVector3<std::common_type_t<T, U>> operator-(const FVector3<U>& v) const noexcept
		{
			return {x - v.x, y - v.y, z - v.z};
		}

		template <class U>
		constexpr FVector3<std::common_type_t<T, U>> operator*(U f) const noexcept
		{
			return {x*f, y*f, z*f};
		}

		template <class U>
		constexpr FVector3<std::common_type_t<T, U>> operator/(U f) const noexcept
		{
			return {x/f, y/f, z/f};
		}

		template <class U>
		constexpr auto operator|(const FVector3<U>& v) const noexcept
		{
			return x*v.x + y*v.y + z*v.z;
		}

		template <class U>
		constexpr FVector3<std::common_type_t<T, U>> operator^(const FVector3<U>& v) const noexcept
		{
			return {y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x};
		}
	};

	template <class T, class U>
	constexpr FVector3<std::common_type_t<T, U>> operator*(U f, const FVector3<T>& v) noexcept
	{
		return v * f;
	}
}
