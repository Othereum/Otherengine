option(OE_USE_AVX2 "Use AVX2?" ON)

option(OE_DEPS_FORCE_INTERNAL "Force dependencies to be downloaded and built directly.")
option(OE_DEPS_FORCE_EXTERNAL "Force to use dependencies installed on the system.")
oe_check_mutex(OE_DEPS_FORCE_INTERNAL OE_DEPS_FORCE_EXTERNAL)

set(OE_OPENGL_VERSION_MAJOR 4 CACHE STRING "")
set(OE_OPENGL_VERSION_MINOR 3 CACHE STRING "")
