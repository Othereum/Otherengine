#pragma once
#include <type_traits>

namespace oeng
{
	struct ZeroThen {};
	struct OneThen {};
	
	namespace detail
	{
		template <class A, class B, bool = std::is_empty_v<A> || std::is_empty_v<B>>
		struct CompressedPairImpl;

		template <class A, class B>
		struct CompressedPairImpl<A, B, true>
		{
			template <class... Arg2>
			explicit CompressedPairImpl(ZeroThen, Arg2&&... arg2):
				first{}, second{std::forward<Arg2>(arg2)...}
			{
			}
			
			template <class Arg1, class... Arg2>
			CompressedPairImpl(OneThen, Arg1&& arg1, Arg2&&... arg2):
				first{std::forward<Arg1>(arg1)}, second{std::forward<Arg2>(arg2)...}
			{
			}

			CompressedPairImpl(const CompressedPairImpl& o):
				first{o.first}, second{o.second}
			{
			}

			CompressedPairImpl(CompressedPairImpl&& o) noexcept:
				first{std::move(o.first)}, second(std::move(o.second))
			{
			}

			~CompressedPairImpl()
			{
				second.~B();
				first.~A();
			}

			CompressedPairImpl& operator=(const CompressedPairImpl& o)
			{
				if (this != &o)
				{
					first = o.first;
					second = o.second;
				}
				return *this;
			}

			CompressedPairImpl& operator=(CompressedPairImpl&& o) noexcept
			{
				if (this != &o)
				{
					first = std::move(o.first);
					second = std::move(o.second);
				}
				return *this;
			}
			
			union
			{
				A first;
				B second;
			};
		};

		template <class A, class B>
		struct CompressedPairImpl<A, B, false>
		{
			template <class... Arg2>
			explicit constexpr CompressedPairImpl(ZeroThen, Arg2&&... arg2):
				first{}, second{std::forward<Arg2>(arg2)...}
			{
			}
			
			template <class Arg1, class... Arg2>
			constexpr CompressedPairImpl(OneThen, Arg1&& arg1, Arg2&&... arg2):
				first{std::forward<Arg1>(arg1)}, second{std::forward<Arg2>(arg2)...}
			{
			}
			
			A first;
			B second;
		};
	}

	template <class A, class B>
	using CompressedPair = detail::CompressedPairImpl<A, B>;
}
