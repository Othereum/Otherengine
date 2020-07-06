#pragma once
#include "Log.hpp"
#include "Platform.hpp"

#define STRINGIZE_IMPL(x) u8#x
#define STRINGIZE(x) STRINGIZE_IMPL(x)
#define LINE_STR STRINGIZE(__LINE__)

#define TEXT_PASTE(x) u8##x
#define TEXT(x) TEXT_PASTE(x)
#define U8FILE TEXT(__FILE__)

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

#define ENSURE(expr) (!!(expr) || (OE_ELOG(u8"Ensure failed: " u8#expr u8", file " U8FILE u8", line " LINE_STR), false))
#define ENSURE_MSG(expr, fmt, ...) (!!(expr) || (OE_ELOG(u8"Ensure failed: " u8#expr u8", " fmt u8", file " U8FILE u8", line " LINE_STR, ##__VA_ARGS__), false))

#define EXPECT(expr) (void)(!!(expr) || (OE_ELOG(u8"Expect failed: " u8#expr u8", file " U8FILE u8", line " LINE_STR), false))
#define EXPECT_MSG(expr, fmt, ...) (void)(!!(expr) || (OE_ELOG(u8"Expect failed: " u8#expr u8", " fmt u8", file " U8FILE u8", line " LINE_STR, ##__VA_ARGS__), false))

#endif

#define OE_ELOG(fmt, ...) (OE_DLOG(1s, ::oeng::log::level::err, fmt, ##__VA_ARGS__), DEBUG_BREAK())
#define EXPECT_NO_ENTRY() EXPECT(!u8"Enclosing block should never be called")
