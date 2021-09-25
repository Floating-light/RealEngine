
add_library(Application)

target_sources(Application
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Private/Application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Application.h
)

target_include_directories(Application 
  PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}"
  )
