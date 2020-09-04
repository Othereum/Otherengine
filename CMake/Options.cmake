option(OE_USE_AVX2 "Use AVX2?" ON)

option(OE_DEPS_FORCE_INTERNAL "Force dependencies to be downloaded and built directly.")
option(OE_DEPS_FORCE_EXTERNAL "Force to use dependencies installed on the system.")
oe_check_mutex(OE_DEPS_FORCE_INTERNAL OE_DEPS_FORCE_EXTERNAL)

set(OE_OPENGL_VERSION_MAJOR 4 CACHE STRING "")
set(OE_OPENGL_VERSION_MINOR 3 CACHE STRING "")

option(OE_NAME_THREADSAFE "Should 'Name' class be thread-safe?" FALSE)
option(OE_PATH_THREADSAFE "Should 'Path' class be thread-safe?" FALSE)
option(OE_LOG_THREADSAFE "Should logging be thread-safe?" FALSE)

option(OE_BUILD_DEBUG "" FALSE)
option(OE_BUILD_DEVELOPMENT "" TRUE)
option(OE_BUILD_TEST "" FALSE)
option(OE_BUILD_SHIPPING "" FALSE)
oe_check_only_one(OE_BUILD_DEBUG OE_BUILD_DEVELOPMENT OE_BUILD_TEST OE_BUILD_SHIPPING)
