# tests/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(jojTest)

add_executable(jojTest main.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_link_directories(jojTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(jojTest PUBLIC joj)
