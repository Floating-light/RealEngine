
message(STATUS "----------->>> source directory : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "----------->>> source directory : ${CMAKE_SOURCE_DIR}")

add_library(Renderer "")

message(STATUS "${Red}---> Current binary dir is ${CMAKE_CURRENT_BINARY_DIR}")

# list(APPEND flags "-permissive")

# add_compile_options(/permissive)

message(STATUS "${Red}---> Current  current falgs  is ${CMAKE_CXX_FLAGS}")
file(GLOB_RECURSE SOURCES_CPP "*.cpp" )
file(GLOB_RECURSE SOURCES_H "*.h" )
file(GLOB_RECURSE SHADERS "*.hlsl")

target_sources(Renderer
  PUBLIC
    ${SOURCES_H}
  PRIVATE
    ${SOURCES_CPP}
    ${SHADERS}
)
source_group("Shaders" FILES ${SHADERS})
set_source_files_properties(${SHADERS}  PROPERTIES  HEADER_FILE_ONLY TRUE)

target_include_directories(Renderer
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

target_link_libraries(Renderer
  PUBLIC
    GraphicInterface 
    Application
    Core
    DirectXMath
    DirectXTex
  )

target_link_libraries(Renderer
  PRIVATE
    GraphicInterface
    dxgi.lib
    d3d12.lib
    User32.lib
    d3dcompiler.lib
    dxguid.lib
)
