# src/resources/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(jojResources STATIC resource_manager.cpp mesh.cpp)

# Specify include directories for jojPlatform
target_include_directories(
    jojResources
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

#add_subdirectory(geometry)
