# src/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(joj STATIC joj.cpp)

# Specify include directories for joj
target_include_directories(
    joj
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/joj
)

add_subdirectory(platform)
add_subdirectory(graphics)

# Specify library directories for joj
target_link_directories(
    joj
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/platform
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics
)

target_link_libraries(joj PUBLIC jojPlatform jojGraphics)
