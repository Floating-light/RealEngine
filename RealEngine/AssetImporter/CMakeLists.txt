add_library(AssetImporter "")

target_sources(AssetImporter
  PRIVATE
    AssetImporter.h
    AssetImporter.cpp
)

target_include_directories(AssetImporter
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
# find_package(assimp 5.2.5 REQUIRED)
set_target_properties(AssetImporter PROPERTIES LINKER_LANGUAGE CXX)
# https://assimp-docs.readthedocs.io/en/latest/usage/use_the_lib.html#introduction
target_link_libraries(AssetImporter
  PRIVATE
    assimp
    Core
    Renderer
)