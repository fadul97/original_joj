# src/resources/geometry/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(jojGeometry STATIC geometry.cpp cube.cpp sphere.cpp cylinder.cpp geo_sphere.cpp grid.cpp quad.cpp)

# Specify include directories for jojGeometry
target_include_directories(
    jojGeometry
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj
)

target_link_directories(
    jojGeometry 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj/math
)

target_link_libraries(jojGeometry PUBLIC jojMath)
