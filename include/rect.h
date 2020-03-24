#pragma once
#include <SDL_rect.h>
#include "Vector.h"

namespace game
{
	template <class T>
	struct FRect;

	using TFRect = FRect<float>;

	template <class T>
	struct FRect
{
		Vector2<T> center, size;

		constexpr FRect() noexcept = default;
		constexpr FRect(Vector2<T> center, Vector2<T> size) noexcept
			:center{center}, size{size}
		{
		}
		
		constexpr FRect(const SDL_Rect& r) noexcept
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