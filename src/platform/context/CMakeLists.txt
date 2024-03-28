# src/graphics/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(jojGraphicsContext STATIC context.cpp)

# Specify include directories for jojGraphicsContext
target_include_directories(
    jojGraphicsContext
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/joj
)

add_subdirectory(opengl)

if(LINUX)
    target_link_directories(
        jojGraphicsContext 
        PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/../
        ${CMAKE_CURRENT_SOURCE_DIR}/opengl/x11
    )

    target_link_libraries(jojGraphicsContext PUBLIC jojPlatform jojX11GLGraphics)

elseif(WIN32)
    add_subdirectory(dx11)
    target_link_directories(
        jojGraphicsContext 
        PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/../
        ${CMAKE_CURRENT_SOURCE_DIR}/opengl/win32
        ${CMAKE_CURRENT_SOURCE_DIR}/dx11
    )

    target_link_libraries(jojGraphicsContext PUBLIC jojPlatform jojWin32GLGraphics jojDX11Graphics)
endif()

