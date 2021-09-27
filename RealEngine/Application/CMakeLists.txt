
add_library(Application)

target_sources(Application
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Private/Application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Application.h
)


# 跨平台
# https://stackoverflow.com/questions/3627127/writing-cross-platform-c-code-windows-linux-and-mac-osx
# https://www.backblaze.com/blog/10-rules-for-how-to-write-cross-platform-code/
# https://stackoverflow.com/questions/3217329/how-to-write-a-cross-platform-program
target_include_directories(Application 
  PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}"
  )

# https://stackoverflow.com/questions/9160335/os-specific-instructions-in-cmake-how-to
# WIN32, UNIX, APPLE

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  message(STATUS "Configuring on/for Windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  message(STATUS "Configuring on/for Linux")
endif()
