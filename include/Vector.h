#pragma once
#include <algorithm>
#include <numeric>
#include <ostream>

namespace game
{
	template <class T, size_t L>
	struct Vector;

	using Vec2 = Vector<float, 2>;
	using Vec3 = Vector<float, 3>;
	using Vec4 = Vector<float, 4>;

	namespace detail
	{
		template <class T, size_t L>
		struct VecBase
		{
			template <class... Args>
			constexpr VecBase(Args... args) noexcept: data{args...} {}
			
			T data[L];
		};

		template <class T>
		struct VecBase<T, 2>
		{
			template <class... Args>
			constexpr VecBase(Args... args) noexcept: data{args...} {}

			union
			{
				T data[2];
				struct { T x, y; };
			};
		};

		template <class T>
		struct VecBase<T, 3>
		{
			template <class... Args>
			constexpr VecBase(Args... args) noexcept: data{args...} {}
			
			union
			{
				T data[3];
				struct { T x, y, z; };
			};
			
			// Cross product
			template <class U>
			constexpr Vector<std::common_type_t<T, U>, 3> operator^(const Vector<U, 3>& v) const noexcept
			{
				return {y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x};
			}
		};

		template <class T>
		struct VecBase<T, 4>
		{
			template <class... Args>
			constexpr VecBase(Args... args) noexcept: data{args...} {}

			union
			{
				T data[4];
				struct { T x, y, z, w; };
			};
		};
	}

	template <class T, size_t L>
	struct Vector : detail::VecBase<T, L>
	{
		template <class... Args>
		constexpr Vector(Args... args) noexcept: detail::VecBase<T, L>{static_cast<T>(args)...} {}

		template <class U, size_t M>
		constexpr Vector(const Vector<U, M>& v) noexcept
		{
			for (size_t i=0; i<L; ++i)
				(*this)[i] = static_cast<T>(v[i]);
		}

		[[nodiscard]] constexpr T LenSqr() const noexcept { return *this | *this; }
		[[nodiscard]] float Len() const noexcept { return sqrtf(static_cast<float>(LenSqr())); }
		
		[[nodiscard]] constexpr T DistSqr(const Vector& v) const noexcept { return (*this - v).LenSqr(); }
		[[nodiscard]] float Dist(const Vector& v) const noexcept { return (*this - v).Len(); }

		void Normalize() noexcept { *this /= Len(); }
		[[nodiscard]] Vector Normal() const noexcept { return *this / Len(); }

		[[nodiscard]] constexpr auto& Data() noexcept { return this->data; }
		[[nodiscard]] constexpr auto& Data() const noexcept { return this->data; }

		constexpr T& operator[](size_t i) noexcept { return this->data[i]; }
		constexpr T operator[](size_t i) const noexcept { return this->data[i]; }

		constexpr bool operator==(const Vector& other) const noexcept
		{
			return std::equal(this->data, this->data + L, other.data);
		}

		template <class Fn>
		constexpr Vector& Transform(const Vector& other, Fn&& fn) noexcept(std::is_nothrow_invocable_v<Fn, T, T>)
		{
			std::transform(this->data, this->data + L, other.data, this->data, std::forward<Fn>(fn));
			return *this;
		}

		template <class Fn>
		constexpr Vector& Transform(Fn&& fn) noexcept(std::is_nothrow_invocable_v<Fn, T>)
		{
			std::transform(this->data, this->data + L, this->data, std::forward<Fn>(fn));
			return *this;
		}

		constexpr Vector& operator+=(const Vector& v) noexcept
		{
			return Transform(v, std::plus<>{});
		}

		constexpr Vector& operator-=(const Vector& v) noexcept
		{
			return Transform(v, std::minus<>{});
		}

		constexpr Vector& operator*=(const Vector& v) noexcept
		{
			return Transform(v, std::multiplies<>{});
		}
		
		constexpr Vector& operator*=(T f) noexcept
		{
			return Transform([f](T v){return v*f;});
		}

		constexpr Vector& operator/=(T f) noexcept
		{
			return Transform([f](T v){return v/f;});
		}

		constexpr Vector operator+(const Vector& v) const noexcept { return Vector{*this} += v; }
		constexpr Vector operator-(const Vector& v) const noexcept { return Vector{*this} -= v; }
		
		template <class U>
		constexpr auto operator*(const Vector<U, L>& v) const noexcept
		{
			Vector<std::common_type_t<T, U>, L> r;
			for (size_t i=0; i<L; ++i) r[i] = (*this)[i] * v[i];
			return r;
		}
		
		constexpr Vector operator*(T f) const noexcept { return Vector{*this} *= f; }
		constexpr Vector operator/(T f) const noexcept { return Vector{*this} /= f; }

		constexpr T operator|(const Vector& v) const noexcept
		{
			T t{};
			for (size_t i = 0; i < L; ++i) t += (*this)[i] * v[i];
			return t;
		}
	};
	
	template <class F, class T, size_t L>
	constexpr Vector<T, L> operator*(F f, const Vector<T, L>& v) noexcept
	{
		return v * f;
	}

	template <class T, size_t L>
	std::ostream& operator<<(std::ostream& os, const Vector<T, L>& v)
	{
		os << v[0];
		for (size_t i = 1; i < L; ++i)
			os << ' ' << v[i];
		return os;
	}
}
