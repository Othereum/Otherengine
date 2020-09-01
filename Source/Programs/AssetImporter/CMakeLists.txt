# Source/Programs/AssetImporter

oe_add_program(AssetImporter)

target_link_libraries(AssetImporter
  PRIVATE
    ${PROJECT_NAME}-Core
)
