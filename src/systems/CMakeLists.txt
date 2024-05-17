# src/systems/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(jojSystems STATIC camera.cpp)

# Specify include directories for jojSystems
target_include_directories(
    jojSystems
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)