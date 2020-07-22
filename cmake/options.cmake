option(OE_BUILD_DOCS "Whether to build documents with doxygen")
option(OE_USE_AVX2 "Use AVX2?" ON)

option(OE_DEPS_FORCE_INTERNAL "Force dependencies to be downloaded and built directly.")
option(OE_DEPS_FORCE_EXTERNAL "Force to use dependencies installed on the system.")
oe_check_mutex(OE_DEPS_FORCE_INTERNAL OE_DEPS_FORCE_EXTERNAL)
