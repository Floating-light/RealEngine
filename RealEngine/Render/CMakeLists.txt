
message(STATUS "----------->>> source directory : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "----------->>> source directory : ${CMAKE_SOURCE_DIR}")

add_library(Render)
# add_library(Renderer "")

message(STATUS "${Red}---> Current binary dir is ${CMAKE_CURRENT_BINARY_DIR}")

# list(APPEND flags "-permissive")

# add_compile_options(/permissive)

message(STATUS "${Red}---> Current  current falgs  is ${CMAKE_CXX_FLAGS}")

target_sources(Render
  PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/AppWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/D3DApp.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GameTime.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Render.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AppWindow.h
    ${CMAKE_CURRENT_LIST_DIR}/D3DApp.h
    ${CMAKE_CURRENT_LIST_DIR}/d3dx12.h
    ${CMAKE_CURRENT_LIST_DIR}/GameTime.h
    ${CMAKE_CURRENT_LIST_DIR}/Helper.h
    ${CMAKE_CURRENT_LIST_DIR}/stdafx.h
)

target_link_libraries(Render
  PRIVATE
    dxgi.lib
    d3d12.lib
    User32.lib
    d3dcompiler.lib
)
