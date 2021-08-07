include(CMakeDependentOption)
include(FetchContent)
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(RealEngine )

# project(RealEngine LANGUAGES CXX)
# 导致链接C的库时找不到C编译器

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.4
)
set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

FetchContent_MakeAvailable(glfw)

# assimp
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
)
# set( BUILD_SHARED_LIBS OFF CACHE BOOL "static lib")
FetchContent_MakeAvailable(assimp)

# glad 
add_library(glad-static
    STATIC
    RealEngine/ThirdParty/glad/glad.cpp
    RealEngine/ThirdParty/glad/glad.h
    )

# Custom library header and lib 
include_directories(./RealEngine/ThirdParty/)
include_directories(./)

LINK_DIRECTORIES(./RealEngine/ThirdParty/lib/)

# error LNK2019: unresolved external symbol WinMain referenced in function "int __cdecl invoke_main(void)" (?invoke_main@@YAHXZ)
# Must after project declare
if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

# CONFIGURE_DEPENDS automatically check and reset the file lists any time the build is invoked.
# avoid manually re-running cmake every time a file is added
file(GLOB_RECURSE allSource CONFIGURE_DEPENDS "utility/*.h" "utility/*.hpp" "utility/*.cpp" "Shaders/*.h" "Shaders/*.cpp" )
list(APPEND allSource main.cpp)
message(STATUS "${allSource}")

add_executable(RealEngine WIN32 ${allSource} )

# 会导致C的链接不到, 比如Windows的API, 干
# set_target_properties(RealEngine PROPERTIES LINKER_LANGUAGE Cxx)
target_link_libraries(RealEngine glad-static glfw ${OPENGL_LIBRARIES} freetype.lib opencv_world451d.lib )
target_link_libraries(RealEngine assimp)