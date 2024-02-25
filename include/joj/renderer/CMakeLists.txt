# include/joj/renderer/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

add_subdirectory(opengl)

if(WIN32)
    add_subdirectory(dx11)
endif()
