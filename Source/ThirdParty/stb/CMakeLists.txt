add_library(stb STATIC stb_image.c)
target_include_directories(stb PUBLIC .)
# oe_optimize_target(stb)
set_property(TARGET stb PROPERTY POSITION_INDEPENDENT_CODE TRUE)
