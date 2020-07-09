#pragma once
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4566)
#endif
#include <fmt/format.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#include "Templates/String.hpp"

namespace oeng
{
	template <class... Args>
	[[nodiscard]] std::u8string Format(std::u8string_view fmt, const Args&... args)
	{
		return fmt::format(fmt, args...);
	}

	template <class T = std::runtime_error, class... Args>
	[[noreturn]] void Throw(std::u8string_view fmt, const Args&... args)
	{
		throw T{AsString(Format(fmt, args...))};
	}

	[[nodiscard]] inline const char8_t* What(const std::exception& e) noexcept
	{
		return AsString8(e.what());
	}
}
