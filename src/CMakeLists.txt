# src/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_library(joj STATIC joj.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include/joj)
