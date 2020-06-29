#pragma once
#include "Log.hpp"
#include "Platform.hpp"
#if !defined(NDEBUG) && !defined(_WIN32)
#include <csignal>
#endif

namespace oeng
{
	inline void DebugBreak() noexcept
	{
#ifndef NDEBUG
		if (!plf::IsDebugging()) return;
#ifdef _WIN32
		__debugbreak();
#else
		std::raise(SIGTRAP);
#endif
#endif
	}
}

#ifdef NDEBUG
	#define ENSURE(expr) !!(expr)
	#define ENSURE_MSG(expr, fmt, ...) !!(expr)

	#define EXPECT(expr) (void)0
	#define EXPECT_MSG(expr, fmt, ...) (void)0
#else
	#define ENSURE(expr) (!!(expr) || (OE_DLOG(1s, ::oeng::log::level::err, "Ensure failed: " #expr ", file " __FILE__ ", line " LINE_STRING), ::oeng::DebugBreak(), false))
	#define ENSURE_MSG(expr, fmt, ...) (!!(expr) || (OE_DLOG(1s, ::oeng::log::level::err, "Ensure failed: " #expr ", " fmt ", file " __FILE__ ", line " LINE_STRING, ##__VA_ARGS__), ::oeng::DebugBreak(), false))

	#define EXPECT(expr) (void)(!!(expr) || (OE_DLOG(1s, ::oeng::log::level::err, "Expect failed: " #expr ", file " __FILE__ ", line " LINE_STRING), ::oeng::DebugBreak(), false))
	#define EXPECT_MSG(expr, fmt, ...) (void)(!!(expr) || (OE_DLOG(1s, ::oeng::log::level::err, "Expect failed: " #expr ", " fmt ", file " __FILE__ ", line " LINE_STRING, ##__VA_ARGS__), ::oeng::DebugBreak(), false))
#endif
