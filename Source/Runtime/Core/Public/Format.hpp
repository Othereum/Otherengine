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
	template <class... Args>
	[[nodiscard]] std::u8string Format(std::u8string_view fmt, const Args&... args)
	{
		return fmt::format(fmt, args...);
	}

	template <class T = std::runtime_error, class... Args>
	[[noreturn]] void Throw(std::u8string_view fmt, const Args&... args)
	{
		auto str = Format(fmt, args...);
		throw T{reinterpret_cast<std::string&&>(str)};
	}

	[[nodiscard]] inline const char8_t* What(const std::exception& e) noexcept
	{
		return reinterpret_cast<const char8_t*>(e.what());
	}
}
