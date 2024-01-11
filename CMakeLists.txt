# Top level CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests)

