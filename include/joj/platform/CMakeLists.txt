# include/joj/platform/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(joj)

# TODO: Turn it into an interface library?
# add_library(jojPlatformInterface INTERFACE window.h)

# target_include_directories(
#     jojPlatformInterface
#     INTERFACE
#     ${CMAKE_CURRENT_SOURCE_DIR}/..
# )

# TODO: if interface, remove 'legacy' include_directories()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)


add_subdirectory(x11)
