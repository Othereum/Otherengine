#pragma once
#include <SDL_rect.h>
#include "vector.h"

namespace game
{
	template <class T>
	struct rect;

	using frect = rect<float>;

	template <class T>
	struct rect
{
		vector2<T> center, size;

		constexpr rect() noexcept = default;
		constexpr rect(vector2<T> center, vector2<T> size) noexcept
			:center{center}, size{size}
		{
		}
		
		constexpr rect(const SDL_Rect& r) noexcept
			:center{T(r.x), T(r.y)}, size{T(r.w), T(r.h)}
		{
			center += size / 2;
		}
		
		constexpr operator SDL_Rect() const noexcept
		{
			const vector2<int> p{center - size/2};
			const vector2<int> s{size};
			return {p.x, p.y, s.x, s.y};
		}
	};
}