#pragma once
#include "Core.hpp"

#define STRINGIZE_IMPL(x) #x
#define STRINGIZE(x) STRINGIZE_IMPL(x)
#define LINE_STRING STRINGIZE(__LINE__)

#if OE_BUILD_CONFIG == 0

#define OE_DO_CHECK 1
#define OE_DO_SLOW_CHECK 1

#elif OE_BUILD_CONFIG == 1

#define OE_DO_CHECK 1
#define OE_DO_SLOW_CHECK 0

#elif OE_BUILD_CONFIG == 2

#define OE_DO_CHECK 0
#define OE_DO_SLOW_CHECK 0

#else

#error OE_BUILD_CONFIG must be either 0, 1, or 2

#endif

#if OE_DO_CHECK

namespace oeng::detail
{
	[[noreturn]] OEAPI void OnAssertionFailed(const char* msg) noexcept;
	OEAPI void OnEnsureFailed(const char* msg) noexcept;
}

#define CHECK_MSG(expr, msg) (void)((!!(expr)) || (::oeng::detail::OnAssertionFailed("Assertion failed: " msg ", file " __FILE__ ", line " LINE_STRING), false))
#define ENSURE_MSG(expr, msg) ((!!(expr)) || (::oeng::detail::OnEnsureFailed("Ensure failed: " msg ", file " __FILE__ ", line " LINE_STRING), false))
#define EXPECT_MSG(expr, msg) (void)((!!(expr)) || (::oeng::detail::OnEnsureFailed("Expect failed: " msg ", file " __FILE__ ", line " LINE_STRING), false))

#else

#define CHECK_MSG(expr, msg) ((void)0)
#define ENSURE_MSG(expr, msg) (!!(expr))
#define EXPECT_MSG(expr, msg) ((void)0)

#endif


#define CHECK(expr) CHECK_MSG(expr, #expr)
#define ENSURE(expr) ENSURE_MSG(expr, #expr)
#define EXPECT(expr) EXPECT_MSG(expr, #expr)
#define IF_ENSURE_MSG(expr, msg) if (ENSURE_MSG(expr, msg)) [[likely]]
#define IF_ENSURE(expr) IF_ENSURE_MSG(expr, #expr)
#define IF_NOT_ENSURE_MSG(expr, msg) if (!ENSURE_MSG(expr, msg)) [[unlikely]]
#define IF_NOT_ENSURE(expr) IF_NOT_ENSURE_MSG(expr, #expr)


#if OE_DO_SLOW_CHECK

#define CHECK_SLOW_MSG(expr, msg) CHECK_MSG(expr, msg)

#else

#define CHECK_SLOW_MSG(expr, msg) ((void)0)

#endif


#define CHECK_SLOW(expr) CHECK_SLOW_MSG(expr, #expr)

