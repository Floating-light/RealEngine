
message(STATUS "----------->>> source directory : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "----------->>> source directory : ${CMAKE_SOURCE_DIR}")

add_library(Renderer "")

message(STATUS "${Red}---> Current binary dir is ${CMAKE_CURRENT_BINARY_DIR}")

# list(APPEND flags "-permissive")

# add_compile_options(/permissive)

message(STATUS "${Red}---> Current  current falgs  is ${CMAKE_CXX_FLAGS}")
file(GLOB_RECURSE SOURCES_CPP "*.cpp" )
file(GLOB_RECURSE SOURCES_H "*.h" )

target_sources(Renderer
  PUBLIC
    ${SOURCES_H}
  PRIVATE
    ${SOURCES_CPP}
)
# target_sources(Renderer
#   PRIVATE 
#     ${CMAKE_CURRENT_LIST_DIR}/Render.cpp
#     ${CMAKE_CURRENT_LIST_DIR}/Render.h
#     ${CMAKE_CURRENT_LIST_DIR}/D3DApp.cpp
#     ${CMAKE_CURRENT_LIST_DIR}/D3DApp.h
#     ${CMAKE_CURRENT_LIST_DIR}/GameTime.cpp
#     ${CMAKE_CURRENT_LIST_DIR}/GameTime.h
#     ${CMAKE_CURRENT_LIST_DIR}/Helper.cpp
#     ${CMAKE_CURRENT_LIST_DIR}/Helper.h
#     PrimitiveInfo.h
#     PrimitiveInfo.cpp
#     SceneInfo.h
#     ViewInfo.h
#     ViewInfo.cpp
#     ModelData.h
#     ModelData.cpp
# )
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
