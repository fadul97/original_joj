# src/renderer/opengl/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(jojGLRenderer STATIC renderer_gl.cpp shader_gl.cpp ebo.cpp vbo.cpp vao.cpp render_object.cpp texture_gl.cpp animation2d_gl.cpp texture_rect_gl.cpp)

# add_library(jojGLRenderer STATIC vao.cpp vbo.cpp ebo.cpp)

target_link_directories(
    jojGLRenderer
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(jojGLRenderer PUBLIC jojRenderer)
