add_library(RHID3D12ThirdParty)

target_include_directories(RHID3D12ThirdParty
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(RHID3D12ThirdParty
  PUBLIC
    d3dx12.h
    D3D12Core.h
    D3D12Core.cpp
  )

target_link_libraries(RHID3D12ThirdParty
  PUBLIC
    dxgi.lib
    d3d12.lib
    User32.lib
    d3dcompiler.lib
)