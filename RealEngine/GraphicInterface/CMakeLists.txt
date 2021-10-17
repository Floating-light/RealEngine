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

endif()

target_link_libraries(GraphicInterface 
  PUBLIC
    Core
  )