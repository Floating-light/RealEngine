
message(STATUS "----------->>> source directory : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "----------->>> source directory : ${CMAKE_SOURCE_DIR}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(colors)
define_colors()

add_executable(RendererMain WIN32 "")
# add_library(Renderer "")

message(STATUS "${Red}---> Current binary dir is ${CMAKE_CURRENT_BINARY_DIR}")

# list(APPEND flags "-permissive")

target_compile_definitions(RendererMain 
  PUBLIC
    UNICODE
    DEBUG
)
# add_compile_options(/permissive)

message(STATUS "${Red}---> Current  current falgs  is ${CMAKE_CXX_FLAGS}")
target_compile_options(RendererMain
PUBLIC
  /std:c++20
  /permissive
  /W1
)

target_sources(RendererMain
  PRIVATE 
    AppWindow.cpp
    D3DApp.cpp
    GameTime.cpp
    Render.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/AppWindow.h
    ${CMAKE_CURRENT_LIST_DIR}/D3DApp.h
    ${CMAKE_CURRENT_LIST_DIR}/d3dx12.h
    ${CMAKE_CURRENT_LIST_DIR}/GameTime.h
    ${CMAKE_CURRENT_LIST_DIR}/Helper.h
    ${CMAKE_CURRENT_LIST_DIR}/stdafx.h
    )
