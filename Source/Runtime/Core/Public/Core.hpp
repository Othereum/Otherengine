#pragma once

#ifdef _WIN32
#define OE_EXPORT __declspec(dllexport)
#define OE_IMPORT __declspec(dllimport)
#else
#if defined(__GNUC__) && __GNUC__>=4
#define OE_IMPORT __attribute__ ((visibility("default")))
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#define OE_IMPORT __global
#else
#define OE_IMPORT
#endif
#define OE_EXPORT OE_IMPORT
#endif

#define EXPLICIT_CPMV(name, spec) \
	name(const name&) = spec; \
	name(name&&) = spec; \
	name& operator=(const name&) = spec; \
	name& operator=(name&&) = spec

#define DELETE_CPMV(name) EXPLICIT_CPMV(name, delete)
#define DEFAULT_CPMV(name) EXPLICIT_CPMV(name, default)

#define INTERFACE_BODY(name) \
	name() = default; \
    virtual ~name() = default; \
    DELETE_CPMV(name)

#define U8_TEXT_IMPL(x) u8##x
#define U8_TEXT(x) U8_TEXT_IMPL(x)
#define U8_FILE U8_TEXT(__FILE__)

#define STRINGIZE_IMPL(x) #x
#define STRINGIZE(x) STRINGIZE_IMPL(x)
#define U8_LINE U8_TEXT(STRINGIZE(__LINE__))

namespace oeng
{
namespace core
{
/**
 * Check if the current thread is a game thread.
 * @return True if called from the game thread.
 */
[[nodiscard]] CORE_API bool IsGameThread() noexcept;
}

using namespace core;
}
