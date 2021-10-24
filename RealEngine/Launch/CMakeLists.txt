add_executable(Launch WIN32)

target_sources(Launch
    PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/RealMain.cpp
      ${CMAKE_CURRENT_LIST_DIR}/Windows/LaunchWindows.cpp
      ${CMAKE_CURRENT_LIST_DIR}/RealMain.h                                                                                                                           
)

# https://stackoverflow.com/questions/42166764/cmake-library-dependencies

# target_include_directories(../Application)
target_link_libraries(Launch 
  PRIVATE
    Core
    Engine
    GraphicInterface
)