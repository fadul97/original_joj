# src/platform/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(jojPlatform STATIC window.cpp)

# Specify include directories for jojPlatform
target_include_directories(
    jojPlatform
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj
)
