#pragma once

#define STRINGIZE_IMPL(x) #x
#define STRINGIZE(x) STRINGIZE_IMPL(x)
#define LINE_STRING STRINGIZE(__LINE__)

#if OE_BUILD_MODE == 0

#define OE_DO_CHECK 1
#define OE_DO_SLOW_CHECK 1

#elif OE_BUILD_MODE == 1

#define OE_DO_CHECK 1
#define OE_DO_SLOW_CHECK 0

#elif OE_BUILD_MODE == 2

#define OE_DO_CHECK 0
#define OE_DO_SLOW_CHECK 0

#else

#error OE_BUILD_MODE must be either 0, 1, or 2

#endif


#if OE_DO_CHECK

namespace oeng::detail
{
	[[noreturn]] void OnAssertionFailed(const char* msg) noexcept;
	void OnEnsureFailed(const char* msg) noexcept;
}

#define CHECK_MSG(expr, msg) (void)((!!(expr)) || (::oeng::detail::OnAssertionFailed("Assertion failed: " msg ", file " __FILE__ ", line " LINE_STRING), 0))
#define CHECK(expr) CHECK_MSG(expr, #expr)
#define ENSURE_MSG(expr, msg) ((!!(expr)) || (::oeng::detail::OnEnsureFailed("Ensure failed: " msg ", file " __FILE__ ", line " LINE_STRING), false))
#define ENSURE(expr) ENSURE_MSG(expr, #expr)

#else

#define CHECK(expr) ((void)0)
#define CHECK_MSG(expr, msg) ((void)0)
#define ENSURE_MSG(expr, msg) (!!(expr))
#define ENSURE(expr) (!!(expr))

#endif


#if OE_DO_SLOW_CHECK

#define CHECK_SLOW(expr) CHECK(expr)
#define CHECK_SLOW_MSG(expr, msg) CHECK_MSG(expr, msg)

#else

#define CHECK_SLOW(expr) ((void)0)
#define CHECK_SLOW_MSG(expr, msg) ((void)0)

#endif


