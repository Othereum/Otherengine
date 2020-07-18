#pragma once

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

#define GAME_API extern "C" OE_EXPORT
#define DEFINE_GAME_MODULE(game_name) GAME_API const char8_t* const kGameName = game_name

#define EXPLICIT_CPMV(name, spec) \
	name(const name&) = spec; \
	name(name&&) = spec; \
	name& operator=(const name&) = spec; \
	name& operator=(name&&) = spec

#define DELETE_CPMV(name) EXPLICIT_CPMV(name, delete)
#define DEFAULT_CPMV(name) EXPLICIT_CPMV(name, default)

#define INTERFACE_BODY(name) \
	constexpr name() noexcept = default; \
	virtual ~name() {} \
	EXPLICIT_CPMV(name, default)

namespace oeng
{
	namespace core {}
	using namespace core;
}

#define U8_TEXT_IMPL(x) u8##x
#define U8_TEXT(x) U8_TEXT_IMPL(x)
#define U8_FILE U8_TEXT(__FILE__)

#define STRINGIZE_IMPL(x) #x
#define STRINGIZE(x) STRINGIZE_IMPL(x)
#define U8_LINE U8_TEXT(STRINGIZE(__LINE__))
