add_library(Core)

target_sources(Core
  PRIVATE
    "Core.h"
    "Core.cpp"

    Module/ModuleManager.h
    Module/ModuleManager.cpp
    Math/Matrix4.h
    Math/Matrix4.cpp
    Math/Math.cpp
    Math/Vector4D.h
    Math/Color.cpp
) 

target_include_directories(Core
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    ${CMAKE_CURRENT_SOURCE_DIR}/Math
)

if(WIN32)
  target_sources(Core
    PRIVATE  
      Windows/WindowsMisc.h
      Windows/WindowsMisc.cpp
      Windows/WindowsPlatformMisc.h
      Windows/WindowsPlatformMisc.cpp

  )
  target_include_directories(Core
    PUBLIC
      "${CMAKE_CURRENT_SOURCE_DIR}/Windows"
  ) 
endif()

add_subdirectory(Logging)

target_link_libraries(Core 
  PUBLIC
    Logging
)

set_target_properties(Core PROPERTIES LINKER_LANGUAGE CXX)
