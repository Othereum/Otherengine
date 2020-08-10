#pragma once

namespace oeng::core
{
	/**
	 * Tag type for value-initializing first, constructing second from remaining args
	 */
	struct ZeroThen
	{
	    explicit ZeroThen() = default;
	};

	/**
	 * Tag type for constructing first from one arg, constructing second from remaining args
	 */
	struct OneThen
	{
	    explicit OneThen() = default;
	};

	/**
	 * Stores a pair of values, deriving from empty first
	 */
	template <class T1, class T2, bool = std::is_empty_v<T1> && !std::is_final_v<T1>>
	class CompressedPair final : T1
	{
	public:
	    template <class... Second>
	    constexpr explicit CompressedPair(ZeroThen, Second&&... args) noexcept(
			std::conjunction_v<std::is_nothrow_default_constructible<T1>, std::is_nothrow_constructible<T2, Second...>>)
			: T1{}, second_{std::forward<Second>(args)...}
	    {
	    }

	    template <class First, class... Second>
	    constexpr CompressedPair(OneThen, First&& first, Second&&... second) noexcept(
			std::conjunction_v<std::is_nothrow_constructible<T1, First>, std::is_nothrow_constructible<T2, Second...>>)
			: T1{std::forward<First>(first)}, second_{std::forward<Second>(second)...}
	    {
	    }

	    constexpr T1& first() noexcept { return *this; }
	    constexpr T2& second() noexcept { return second_; }
	    constexpr const T1& first() const noexcept { return *this; }
	    constexpr const T2& second() const noexcept { return second_; }

	private:
	    T2 second_;
	};

	/**
	 * Stores a pair of values, not deriving from first
	 */
	template <class T1, class T2>
	class CompressedPair<T1, T2, false> final
	{
	public:
	    template <class... Second>
	    constexpr explicit CompressedPair(ZeroThen, Second&&... second) noexcept(
			std::conjunction_v<std::is_nothrow_default_constructible<T1>, std::is_nothrow_constructible<T2, Second...>>)
			: first_{}, second_{std::forward<Second>(second)...}
	    {
	    }

	    template <class First, class... Second>
	    constexpr CompressedPair(OneThen, First&& first, Second&&... second) noexcept(
	        std::conjunction_v<std::is_nothrow_constructible<T1, First>, std::is_nothrow_constructible<T2, Second...>>)
			: first_{std::forward<First>(first)}, second_{std::forward<Second>(second)...}
	    {
	    }

	    constexpr T1& first() noexcept { return first_; }
	    constexpr T2& second() noexcept { return second_; }
	    constexpr const T1& first() const noexcept { return first_; }
	    constexpr const T2& second() const noexcept { return second_; }

	private:
	    T1 first_;
	    T2 second_;
	};
}