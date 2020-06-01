#pragma once

#ifdef _WIN32
# if OE_BUILD
#  define OEAPI __declspec(dllexport)
# else
#  define OEAPI __declspec(dllimport)
# endif
#else
# if defined(__GNUC__) && __GNUC__>=4
#  define OEAPI __attribute__ ((visibility("default")))
# elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#  define OEAPI __global
# else
#  define OEAPI
# endif
#endif
