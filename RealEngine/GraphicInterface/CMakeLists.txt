add_library(GraphicInterface "")

file(GLOB_RECURSE SOURCES_CPP "*.cpp" )
file(GLOB_RECURSE SOURCES_H "*.h" )
target_sources(GraphicInterface
  PUBLIC
    ${SOURCES_H}
  PRIVATE
    ${SOURCES_CPP}
)

target_include_directories(GraphicInterface
  PUBLIC
    "${CMKAE_CURRENT_SOURCE_DIR}")

if(WIN32)
  target_sources(GraphicInterface
    PRIVATE
    Windows/WindowsPlatformRHI.cpp
  )
  # target_link_libraries(GraphicInterface 
  # PUBLIC
  #   RHID3D12
  # )
  # Work, must be public, 
  # target_link_options(GraphicInterface
  #   PUBLIC
  #     "LINKER:/include:ForceStaticLibraryReference_D3D12RHI"
  # )
endif()

target_link_libraries(GraphicInterface 
  PUBLIC
    Core
  )
target_link_libraries(GraphicInterface
  PRIVATE
    dxgi.lib
    d3d12.lib
    User32.lib
    d3dcompiler.lib
  )