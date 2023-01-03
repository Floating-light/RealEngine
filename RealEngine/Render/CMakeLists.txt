
message(STATUS "----------->>> source directory : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "----------->>> source directory : ${CMAKE_SOURCE_DIR}")

add_library(Renderer "")

message(STATUS "${Red}---> Current binary dir is ${CMAKE_CURRENT_BINARY_DIR}")

# list(APPEND flags "-permissive")

# add_compile_options(/permissive)

message(STATUS "${Red}---> Current  current falgs  is ${CMAKE_CXX_FLAGS}")

target_sources(Renderer
  PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/Render.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Render.h
    ${CMAKE_CURRENT_LIST_DIR}/D3DApp.cpp
    ${CMAKE_CURRENT_LIST_DIR}/D3DApp.h
    ${CMAKE_CURRENT_LIST_DIR}/GameTime.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GameTime.h
    ${CMAKE_CURRENT_LIST_DIR}/Helper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Helper.h
    PrimitiveInfo.h
    SceneInfo.h
    ViewInfo.h
    ViewInfo.cpp
    Device.h
    Device.cpp
    Viewport.h
    Viewport.cpp
    
    # ${CMAKE_CURRENT_LIST_DIR}/d3dx12.h
    # ${CMAKE_CURRENT_LIST_DIR}/Helper.h
    # ${CMAKE_CURRENT_LIST_DIR}/stdafx.h
)

target_include_directories(Renderer
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

target_link_libraries(Renderer
  PUBLIC
    GraphicInterface 
    Application
    Core
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
