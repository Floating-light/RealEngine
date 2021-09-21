cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(RealEngine)

if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

set(CMAKE_CXX_STANDARD 20)

# set target path  :
# https://stackoverflow.com/questions/13556885/how-to-change-the-executable-output-directory-for-win32-builds-in-cmake

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(RealEngine/Render)
