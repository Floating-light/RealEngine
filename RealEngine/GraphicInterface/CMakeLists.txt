add_library(GraphicInterface "")

target_sources(GraphicInterface 
  PRIVATE
    GraphicInterface.h
    GraphicInterface.cpp
    GraphicViewport.h
    GraphicViewport.cpp
)

target_include_directories(GraphicInterface
  PUBLIC
    "${CMKAE_CURRENT_SOURCE_DIR}")

if(WIN32)
  target_sources(GraphicInterface
    PRIVATE
    Windows/WindowsPlatformRHI.cpp
  )
  target_link_libraries(GraphicInterface 
  PUBLIC
    RHID3D12
  )
  # Work, must be public, 
  target_link_options(GraphicInterface
    PUBLIC
      "LINKER:/include:ForceStaticLibraryReference_D3D12RHI"
  )
endif()

target_link_libraries(GraphicInterface 
  PUBLIC
    Core
  )