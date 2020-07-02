#pragma once
#include "Log.hpp"
#include "Platform.hpp"

#ifdef NDEBUG

#define DEBUG_BREAK()

#define ENSURE(expr) !!(expr)
#define ENSURE_MSG(expr, fmt, ...) !!(expr)

#define EXPECT(expr) (void)0
#define EXPECT_MSG(expr, fmt, ...) (void)0

#else

#ifdef _WIN32
#define DEBUG_BREAK() (void)(plf::IsDebugging() && (__debugbreak(), true))
#else
#include <csignal>
#define DEBUG_BREAK() (void)(plf::IsDebugging() && (std::raise(SIGTRAP), true))
#endif

#define ENSURE(expr) (!!(expr) || (OE_ELOG("Ensure failed: " #expr ", file " __FILE__ ", line " LINE_STRING), false))
#define ENSURE_MSG(expr, fmt, ...) (!!(expr) || (OE_ELOG("Ensure failed: " #expr ", " fmt ", file " __FILE__ ", line " LINE_STRING, ##__VA_ARGS__), false))

#define EXPECT(expr) (void)(!!(expr) || (OE_ELOG("Expect failed: " #expr ", file " __FILE__ ", line " LINE_STRING), false))
#define EXPECT_MSG(expr, fmt, ...) (void)(!!(expr) || (OE_ELOG("Expect failed: " #expr ", " fmt ", file " __FILE__ ", line " LINE_STRING, ##__VA_ARGS__), false))

#endif

#define OE_ELOG(fmt, ...) (OE_DLOG(1s, ::oeng::log::level::err, fmt, ##__VA_ARGS__), DEBUG_BREAK())
#define EXPECT_NO_ENTRY() EXPECT(!"Enclosing block should never be called")
