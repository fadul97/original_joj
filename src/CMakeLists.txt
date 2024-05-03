# src/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(joj STATIC joj.cpp error.cpp)

# Specify include directories for joj
target_include_directories(
    joj
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/joj
)

add_subdirectory(vendor)
add_subdirectory(platform)
#add_subdirectory(renderer)
#add_subdirectory(resources)
#add_subdirectory(systems)

# Specify library directories for joj
target_link_directories(
    joj
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/platform
#    ${CMAKE_CURRENT_SOURCE_DIR}/math
#    ${CMAKE_CURRENT_SOURCE_DIR}/renderer
#    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
#    ${CMAKE_CURRENT_SOURCE_DIR}/systems
)

target_link_libraries(joj PUBLIC jojPlatform jojSTBImage)
