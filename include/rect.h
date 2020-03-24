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
		FVector2<T> center, size;

		constexpr FRect() noexcept = default;
		constexpr FRect(FVector2<T> center, FVector2<T> size) noexcept
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
			const FVector2<int> p{center - size/2};
			const FVector2<int> s{size};
			return {p.x, p.y, s.x, s.y};
		}
	};
}