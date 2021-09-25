add_library(Engine)

target_sources(Engine
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Private/Engine.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Engine.h
)

target_include_directories(Engine
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)