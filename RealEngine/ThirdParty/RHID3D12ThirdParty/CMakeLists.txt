add_library(RHID3D12ThirdParty)

target_include_directories(RHID3D12ThirdParty
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(RHID3D12ThirdParty
  PUBLIC
    d3dx12.h
  )
set_target_properties(RHID3D12ThirdParty PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(RHID3D12ThirdParty
  PRIVATE
    dxgi.lib
    d3d12.lib
    User32.lib
    d3dcompiler.lib
)