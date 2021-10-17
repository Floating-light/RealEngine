add_library(Core)

target_sources(Core
  PRIVATE
    "Core.h"
    "Core.cpp"
) 

target_include_directories(Core
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

if(WIN32)
  target_sources(Core
    PRIVATE  
      "Windows/WindowsMisc.h"
      "Windows/WindowsMisc.cpp"
  )
  target_include_directories(Core
    PUBLIC
      "${CMAKE_CURRENT_SOURCE_DIR}/Windows"
  ) 
endif()

set_target_properties(Core PROPERTIES LINKER_LANGUAGE CXX)