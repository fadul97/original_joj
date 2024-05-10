# src/platform/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(jojPlatform STATIC window.cpp input.cpp timer.cpp)

add_subdirectory(context)

# Specify include directories for jojPlatform
target_include_directories(
    jojPlatform
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/joj/
)

if(LINUX)
    add_subdirectory(x11)
    target_link_directories(
        jojPlatform 
        PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/x11
#        ${CMAKE_CURRENT_SOURCE_DIR}/context
    )
    target_link_libraries(jojPlatform PUBLIC joj jojX11Platform)

elseif(WIN32)
    add_subdirectory(win32)
    target_link_directories(
        jojPlatform
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../
        ${CMAKE_CURRENT_SOURCE_DIR}/win32/
        ${CMAKE_CURRENT_SOURCE_DIR}/context/
    )
    target_link_libraries(jojPlatform PUBLIC joj jojWin32Platform jojGraphicsContext)
endif()

