# src/platform/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(jojPlatform STATIC window.cpp input.cpp timer.cpp platform_manager.cpp)

# Specify include directories for jojPlatform
target_include_directories(
    jojPlatform
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj
)

add_subdirectory(x11)

target_link_directories(jojPlatform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/x11)
target_link_libraries(jojPlatform PUBLIC jojX11Platform)
