# src/graphics/x11/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(jojX11GLGraphics STATIC context_gl.cpp joj_gl_x11.cpp)

target_link_directories(
    jojX11GLGraphics 
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(jojX11GLGraphics PUBLIC jojGraphics -lGL)
