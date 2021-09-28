add_library(Core)

target_sources(Core
  PRIVATE
    "WindowsMisc.h"
)
set_target_properties(Core PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Core
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)