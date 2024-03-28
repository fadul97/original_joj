# src/graphics/dx11/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(jojDX11Graphics STATIC context_dx11.cpp)

target_link_directories(
    jojDX11Graphics
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(jojDX11Graphics PUBLIC dxgi.lib d3d11.lib jojGraphicsContext)
