cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(RealEngine)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

set(CMAKE_CXX_STANDARD 20)

# https://stackoverflow.com/questions/61386679/how-to-get-static-library-default-by-cmake
# 如果不设置这个，就可能会Build Static library，如果没有导出符号，则不会生成.lib文件，这会导致链接错误
option(BUILD_SHARED_LIBS "" OFF)

# set target path  :
# https://stackoverflow.com/questions/13556885/how-to-change-the-executable-output-directory-for-win32-builds-in-cmake

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# https://stackoverflow.com/questions/9017573/define-preprocessor-macro-through-cmake
add_compile_definitions(    
  UNICODE
  DEBUG
)

add_subdirectory(RealEngine)
