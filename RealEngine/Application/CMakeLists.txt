
add_library(Application "")

message(STATUS "-------------->>> ${CMAKE_CURRENT_SOURCE_DIR}/GenericPlatform")
# message(STATUS "-------------->>> ${CMAKE_SOURCE_DIR}")
# message(STATUS "-------------->>> ${PROJECT_SOURCE_DIR}")



# 给编译器的include path, 并不影响Cmake 解析target_source, 它只会把相对路径解析成相对CMAKE_CURRENT_SOURCE_DIR.
target_include_directories(Application
  PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}"
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/GenericPlatform"
)

target_sources(Application
  PRIVATE
    Private/Application.cpp
    Application.h
    GenericPlatform/GenericApplication.h
    GenericPlatform/GenericPlatformFactory.h
    GenericPlatform/GenericWindow.h
)

# 跨平台
# https://stackoverflow.com/questions/3627127/writing-cross-platform-c-code-windows-linux-and-mac-osx
# https://www.backblaze.com/blog/10-rules-for-how-to-write-cross-platform-code/
# https://stackoverflow.com/questions/3217329/how-to-write-a-cross-platform-program

if(WIN32)
  target_sources(Application 
    PRIVATE
      WindowPlatform/WindowsApplication.cpp
      WindowPlatform/WindowsWindow.cpp
      WindowPlatform/WindowsFactory.cpp
      WindowPlatform/WindowsApplication.h
      WindowPlatform/WindowsWindow.h
  )
  target_include_directories(Application
    PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/WindowPlatform"
  )
  
  target_link_libraries(Application 
    PRIVATE
      ${CMAKE_SOURCE_DIR}/resources/win.res
)
endif()

# https://stackoverflow.com/questions/9160335/os-specific-instructions-in-cmake-how-to
# WIN32, UNIX, APPLE

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  message(STATUS "Configuring on/for Windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  message(STATUS "Configuring on/for Linux")
endif()

target_link_libraries(Application
  PRIVATE
    Core
)