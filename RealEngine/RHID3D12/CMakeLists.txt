add_library(RHID3D12 "")

target_sources(RHID3D12
  PRIVATE
    RHID3D12.h
    RHID3D12.cpp
    D3D12Viewport.h
    D3D12Viewport.cpp
)



target_link_libraries(RHID3D12
  PRIVATE
    GraphicInterface
)