# src/renderer/opengl/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(jojGLRenderer STATIC renderer_gl.cpp)

target_link_directories(
    jojGLRenderer
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(jojGLRenderer PUBLIC jojRenderer)
