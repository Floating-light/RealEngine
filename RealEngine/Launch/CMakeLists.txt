add_executable(${PROJECT_NAME} WIN32)
message(STATUS "Project name -------------->>> ${PROJECT_NAME}")

target_sources(${PROJECT_NAME}
    PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/RealMain.cpp
      ${CMAKE_CURRENT_LIST_DIR}/Windows/LaunchWindows.cpp
      ${CMAKE_CURRENT_LIST_DIR}/RealMain.h                                                                                                                           
)

# https://stackoverflow.com/questions/42166764/cmake-library-dependencies

# target_include_directories(../Application)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    Core
    Engine
    GraphicInterface
)