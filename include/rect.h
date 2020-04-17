#pragma once
#include <SDL_rect.h>
#include "Vector.h"

namespace game
{
	template <class T>
	struct Rect;

	using FRect = Rect<float>;

	template <class T>
	struct Rect
{
		Vector<T, 2> center, size;

		constexpr Rect() noexcept = default;
		constexpr Rect(Vector<T, 2> center, Vector<T, 2> size) noexcept
			:center{center}, size{size}
		{
		}
		
		constexpr Rect(const SDL_Rect& r) noexcept
			:center{T(r.x), T(r.y)}, size{T(r.w), T(r.h)}
		{
			center += size / 2;
		}
		
		constexpr operator SDL_Rect() const noexcept
		{
			const Vector<int, 2> p{center - size/2};
			const Vector<int, 2> s{size};
			return {p.x, p.y, s.x, s.y};
		}
	};
}