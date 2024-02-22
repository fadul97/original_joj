# include/joj/graphics/opengl/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

if(LINUX)
    add_subdirectory(x11)
elseif(WIN32)
    add_subdirectory(win32)
endif()
