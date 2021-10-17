add_library(RHID3D12 "")

target_sources(RHID3D12
  PRIVATE
    RHID3D12.h
    RHID3D12.cpp
)



target_link_libraries(RHID3D12
  PRIVATE
    GraphicInterface
)