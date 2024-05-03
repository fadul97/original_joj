# include/joj/math/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(jojMath INTERFACE .)
target_include_directories(jojMath
    INTERFACE
    .
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

if (WIN32)
    target_link_libraries(jojMath INTERFACE dxgi.lib d3d11.lib)
endif ()
