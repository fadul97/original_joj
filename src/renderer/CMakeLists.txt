# src/renderer/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(jojRenderer STATIC renderer.cpp shader.cpp viewport.cpp)

# Specify include directories for jojRenderer
target_include_directories(
    jojRenderer
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

add_subdirectory(opengl)

if(LINUX)
    target_link_directories(
        jojRenderer 
        PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/../platform/
        ${CMAKE_CURRENT_SOURCE_DIR}/opengl/
    )

    target_link_libraries(jojRenderer PUBLIC jojPlatform jojGLRenderer)
elseif(WIN32)
#    add_subdirectory(dx11)
    
    target_link_directories(
        jojRenderer 
        PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/../platform/
        ${CMAKE_CURRENT_SOURCE_DIR}/opengl/
#        ${CMAKE_CURRENT_SOURCE_DIR}/dx11/
    )

    target_link_libraries(jojRenderer PUBLIC jojPlatform jojGLRenderer)

endif()
