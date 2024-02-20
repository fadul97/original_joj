# include/joj/graphics/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

if(LINUX)
    add_subdirectory(x11)
endif()
