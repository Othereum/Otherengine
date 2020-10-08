#pragma once

namespace otm
{
	constexpr auto kHashOffsetBasis = []
	{
		if constexpr (sizeof(size_t) == 8) { return 14695981039346656037ull; }
		else if constexpr (sizeof(size_t) == 4) { return 2166136261u; }
	}();
	
	constexpr auto kHashPrime = []
	{
		if constexpr (sizeof(size_t) == 8) { return 1099511628211ull; }
		else if constexpr (sizeof(size_t) == 4) { return 16777619u; }
	}();

	template <class It, class Fn>
	constexpr size_t HashRange(size_t val, It first, It last, Fn modifier) noexcept
	{
	    for (; first != last; ++first)
		{
	        val ^= modifier(*first);
	        val *= kHashPrime;
	    }
	    return val;
	}

	template <class It>
	constexpr size_t HashRange(size_t val, It first, It last) noexcept
	{
	    return HashRange(val, first, last, []<class T>(T&& x){return std::forward<T>(x);});
	}

	template <class It, class Fn>
	constexpr size_t HashRange(It first, It last, Fn modifier) noexcept
	{
		return HashRange(kHashOffsetBasis, first, last, modifier);
	}

	template <class It>
	constexpr size_t HashRange(It first, It last) noexcept
	{
		return HashRange(kHashOffsetBasis, first, last);
	}
}
