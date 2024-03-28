# src/graphics/opengl/win32/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(jojWin32GLGraphics STATIC context_gl.cpp joj_gl_win32.cpp)

target_link_directories(
    jojWin32GLGraphics 
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

# Find the OpenGL package
find_package(OpenGL REQUIRED)

# Include the OpenGL headers directory
target_include_directories(jojWin32GLGraphics PRIVATE ${OPENGL_INCLUDE_DIR})

# Link OpenGL and JojPlatform to JojGraphicsGL
target_link_libraries(jojWin32GLGraphics PUBLIC ${OPENGL_LIBRARY} jojGraphicsContext)
