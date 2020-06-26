// ReSharper disable CppClangTidyCppcoreguidelinesMacroUsage
// ReSharper disable CppClangTidyBugproneMacroParentheses
#pragma once

#if OE_BUILD_STATIC
	#define OEAPI
	#define OE_EXPORT
	#define OE_IMPORT
#else
	#ifdef _WIN32
		#define OE_EXPORT __declspec(dllexport)
		#define OE_IMPORT __declspec(dllimport)

		#ifdef OE_BUILD
			#define OEAPI OE_EXPORT
		#else
			#define OEAPI OE_IMPORT
		#endif
	#else
		#if defined(__GNUC__) && __GNUC__>=4
			#define OEAPI __attribute__ ((visibility("default")))
		#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
			#define OEAPI __global
		#else
			#define OEAPI
		#endif

		#define OE_IMPORT OEAPI
		#define OE_EXPORT OEAPI
	#endif
#endif

#define OE_EXPLICIT_CPMV(name, spec) \
	name(const name&) = spec; \
	name(name&&) = spec; \
	name& operator=(const name&) = spec; \
	name& operator=(name&&) = spec

#define OE_DELETE_CPMV(name) OE_EXPLICIT_CPMV(name, delete)

#define OE_INTERFACE_BODY(name) \
	constexpr name() noexcept = default; \
	virtual ~name() {} \
	OE_EXPLICIT_CPMV(name, default)

namespace oeng
{
	[[nodiscard]] OEAPI bool IsGameThread() noexcept;
	[[nodiscard]] OEAPI bool IsEngineExists() noexcept;
	[[nodiscard]] OEAPI bool IsDebugging() noexcept;
}
