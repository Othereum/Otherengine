#pragma once
#include "Log.hpp"
#include <csignal>

namespace oeng
{
	inline void DebugBreak() noexcept
	{
#ifndef NDEBUG
		if (!IsDebugging()) return;
#ifdef _WIN32
		__debugbreak();
#else
		std::raise(SIGTRAP);
#endif
#endif
	}
	
	template <class... Args>
	void LogErrorAndBreak(std::string_view fmt, const Args&... args) noexcept
	{
		try { log::Error(fmt, args...); }
		catch (...) {}
		DebugBreak();
	}
}

#ifdef NDEBUG
	#define ENSURE(expr) !!(expr)
	#define ENSURE_MSG(expr, fmt, ...) !!(expr)

	#define EXPECT(expr) (void)0
	#define EXPECT_MSG(expr, fmt, ...) (void)0
#else
	#define ENSURE(expr) (!!(expr) || (::oeng::LogErrorAndBreak("Ensure failed: " #expr ", file " __FILE__ ", line " LINE_STRING), false))
	#define ENSURE_MSG(expr, fmt, ...) (!!(expr) || (::oeng::LogErrorAndBreak("Ensure failed: " #expr ", " fmt ", file " __FILE__ ", line " LINE_STRING, ##__VA_ARGS__), false))

	#define EXPECT(expr) (void)(!!(expr) || (::oeng::LogErrorAndBreak("Expect failed: " #expr ", file " __FILE__ ", line " LINE_STRING), false))
	#define EXPECT_MSG(expr, fmt, ...) (void)(!!(expr) || (::oeng::LogErrorAndBreak("Expect failed: " #expr ", " fmt ", file " __FILE__ ", line " LINE_STRING, ##__VA_ARGS__), false))
#endif
