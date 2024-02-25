# src/graphics/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(jojGraphics STATIC context.cpp)

# Specify include directories for jojGraphics
target_include_directories(
    jojGraphics
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj
)

add_subdirectory(opengl)

if(LINUX)
    target_link_directories(
        jojGraphics 
        PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/../platform
        ${CMAKE_CURRENT_SOURCE_DIR}/opengl/x11
    )

    target_link_libraries(jojGraphics PUBLIC jojPlatform jojX11GLGraphics)
elseif(WIN32)
    add_subdirectory(dx11)
    target_link_directories(
        jojGraphics 
        PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/../platform
        ${CMAKE_CURRENT_SOURCE_DIR}/opengl/win32
        ${CMAKE_CURRENT_SOURCE_DIR}/dx11
    )

    target_link_libraries(jojGraphics PUBLIC jojPlatform jojWin32GLGraphics jojDX11Graphics)
endif()

