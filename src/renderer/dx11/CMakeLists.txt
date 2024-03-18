# src/renderer/dx11/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(jojDX11Renderer STATIC 
    renderer_dx11.cpp 
    swapchain_dx11.cpp 
    viewport_dx11.cpp 
    depth_stencil_manager_dx11.cpp 
    blend_state_dx11.cpp 
    rasterizer_dx11.cpp 
    shader_dx11.cpp 
    vertex_buffer_dx11.cpp
)

target_link_directories(
    jojDX11Renderer
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(jojDX11Renderer PUBLIC jojRenderer dxgi.lib d3d11.lib D3DCompiler.lib)
