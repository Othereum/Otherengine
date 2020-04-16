#pragma once
#include <numeric>

namespace game
{
	template <class T, size_t L>
	struct Vector;

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
		constexpr Vector(Args... args) noexcept: detail::VecBase<T, L>{T(args)...} {}

		template <class U, size_t M>
		constexpr explicit Vector(const Vector<U, M>& v) noexcept
		{
			std::copy(v.data, v.data + std::min(L, M), this->data);
		}

		[[nodiscard]] constexpr T LenSqr() const noexcept { return *this | *this; }
		[[nodiscard]] float Len() const noexcept { return sqrtf(float(LenSqr())); }
		
		[[nodiscard]] constexpr T DistSqr(const Vector& v) const noexcept { return (*this - v).LenSqr(); }
		[[nodiscard]] float Dist(const Vector& v) const noexcept { return (*this - v).Len(); }

		void Normalize() noexcept { *this /= Len(); }
		[[nodiscard]] auto Normal() const noexcept { return *this / Len(); }

		constexpr T& operator[](size_t i) noexcept { return this->data[i]; }
		constexpr T operator[](size_t i) const noexcept { return this->data[i]; }

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

		constexpr Vector& operator+=(const Vector& v)& noexcept
		{
			return Transform(v, std::plus<>{});
		}

		constexpr Vector& operator-=(const Vector& v)& noexcept
		{
			return Transform(v, std::minus<>{});
		}

		constexpr Vector& operator*=(const Vector& v)& noexcept
		{
			return Transform(v, std::multiplies<>{});
		}
		
		constexpr Vector& operator*=(T f)& noexcept
		{
			return Transform([f](T v){return v*f;});
		}

		constexpr Vector& operator/=(T f)& noexcept
		{
			return Transform([f](T v){return v/f;});
		}

		constexpr Vector operator+(const Vector& v) const noexcept { auto t = *this; return t += v; }
		constexpr Vector operator-(const Vector& v) const noexcept { auto t = *this; return t -= v; }
		constexpr Vector operator*(const Vector& v) const noexcept { auto t = *this; return t *= v; }
		constexpr Vector operator*(T f) const noexcept { auto t = *this; return t *= f; }
		constexpr Vector operator/(T f) const noexcept { auto t = *this; return t /= f; }

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
}
