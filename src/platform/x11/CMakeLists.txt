# src/platform/x11/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(jojX11Platform STATIC window_x11.cpp input_x11.cpp timer_x11.cpp platform_manager_x11.cpp)

target_link_directories(
    jojX11Platform 
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../graphics/
)

target_link_libraries(jojX11Platform PUBLIC jojPlatform -lX11)
