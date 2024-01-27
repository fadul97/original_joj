# Top level CMakeLists.txt
cmake_minimum_required(VERSION 3.27)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Setup C++
set(CMAKE_CXX_STANDARD 20)
if(CMAKE_COMPILER_IS_GNUCXX)
	add_compile_options(-pedantic -Wall -Wextra -Wfloat-equal
		-Wunused-parameter -Wunused-variable -Wcast-align -Wcast-qual 
        -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs 
        -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=5 
        -Wswitch-default -Wswitch-enum -Wconversion -Wundef -Werror -Wno-unused 
        -Wpointer-arith -Wstrict-prototypes -Wwrite-strings)
endif()

project(
	joj 
	VERSION 0.0.1
	LANGUAGES CXX)
	
# Update the submodules here
include(cmake/UpdateSubmodules.cmake)

# Enable testing for this project
include(CTest)

add_subdirectory(external)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(samples)

