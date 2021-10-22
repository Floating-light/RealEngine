add_library(RHID3D12 "")

target_sources(RHID3D12
  PRIVATE
    RHID3D12.h
    RHID3D12.cpp
    D3D12Viewport.h
    D3D12Viewport.cpp
    D3D12Adapter.h
    D3D12Adapter.cpp
)

target_include_directories(RHID3D12
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"  
)

target_link_libraries(RHID3D12
  PRIVATE
    GraphicInterface
    dxgi.lib
    d3d12.lib
    User32.lib
    d3dcompiler.lib
)