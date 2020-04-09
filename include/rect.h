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
		Vector2<T> center, size;

		constexpr Rect() noexcept = default;
		constexpr Rect(Vector2<T> center, Vector2<T> size) noexcept
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
			const Vector2<int> p{center - size/2};
			const Vector2<int> s{size};
			return {p.x, p.y, s.x, s.y};
		}
	};
}