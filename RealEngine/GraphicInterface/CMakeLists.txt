add_library(GraphicInterface "")

target_sources(GraphicInterface 
  PRIVATE
    D3D12ThirdPart.h
    GraphicInterface.h
    GraphicInterface.cpp
    GraphicViewport.h
    GraphicViewport.cpp
    CommandContext.h
    CommandContext.cpp
    Adapter.h
    Adapter.cpp
    CommandListManager.h
    CommandListManager.cpp
    CommandAllocatorPool.h
    CommandAllocatorPool.cpp
    RHIResource.h
    RHIBuffer.h
    RHIBuffer.cpp
    RHIUploadBuffer.h
    RHIUploadBuffer.cpp
    DescriptorHeap.h
    DescriptorHeap.cpp
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