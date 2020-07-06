#pragma once
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4566)
#endif
#include <fmt/format.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace oeng
{
	namespace detail
	{
		template <class S>
		concept FmtStr = std::is_same_v<fmt::char_t<S>, char8_t>;
	}
	
	template <detail::FmtStr S, typename... Args>
	[[nodiscard]] auto Format(const S& fmt, Args&&... args)
	{
		return fmt::format(fmt, std::forward<Args>(args)...);
	}

	template <class T = std::runtime_error, detail::FmtStr S, typename... Args>
	[[noreturn]] void Throw(const S& fmt, Args&&... args)
	{
		auto str = Format(fmt, std::forward<Args>(args)...);
		throw T{reinterpret_cast<std::string&&>(str)};
	}

	[[nodiscard]] inline const char8_t* What(const std::exception& e) noexcept
	{
		return reinterpret_cast<const char8_t*>(e.what());
	}
}
