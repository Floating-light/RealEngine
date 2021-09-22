cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

add_library(Application)

target_compile_definitions(Application 
  PUBLIC
    UNICODE
    DEBUG
)

target_sources(Application
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Application.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_FIR}Application.h
)