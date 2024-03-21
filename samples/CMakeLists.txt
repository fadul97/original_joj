# samples/CMakeLists.txt
cmake_minimum_required(VERSION 3.27)
project(jojTest)

add_executable(jojTest main.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_link_directories(jojTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(jojTest PUBLIC joj)

# TODO: Find a better way to:
# Copy shaders files to samples/shaders/
configure_file(shaders/vertex.glsl shaders/vertex.glsl COPYONLY)
configure_file(shaders/frag.glsl shaders/frag.glsl COPYONLY)
configure_file(shaders/vert.hlsl shaders/vert.hlsl COPYONLY)
configure_file(shaders/pixel.hlsl shaders/pixel.hlsl COPYONLY)

if (WIN32)
	configure_file(shaders/vert.hlsl samples/Debug/shaders/vert.hlsl COPYONLY)
	configure_file(shaders/pixel.hlsl samples/Debug/shaders/pixel.hlsl COPYONLY)
	
	# For Visual Studio compilation
	# joj\out\build\x64-Debug\samples\shaders
	configure_file(shaders/vert.hlsl shaders/vert.hlsl COPYONLY)
	configure_file(shaders/pixel.hlsl shaders/pixel.hlsl COPYONLY)
	configure_file(shaders/vert.hlsl vert.hlsl COPYONLY)
	configure_file(shaders/pixel.hlsl pixel.hlsl COPYONLY)

	# "${projectDir}\\out\\build\\${name}"
	set(name "x64-Debug")
	configure_file(shaders/vert.hlsl ${projectDir}\\out\\build\\${name}\\vert.hlsl COPYONLY)
	configure_file(shaders/pixel.hlsl ${projectDir}\\out\\build\\${name}\\pixel.hlsl COPYONLY)
endif()

# Textures from kenney (https://www.kenney.nl/) and itch.io (https://itch.io/game-assets/free/tag-animation)
# TODO: Find a better way to:
# Copy texture files to samples/textures/
configure_file(textures/container.jpg textures/container.jpg COPYONLY)
configure_file(textures/awesomeface.png textures/awesomeface.png COPYONLY)
configure_file(textures/spritesheet.png textures/spritesheet.png COPYONLY)
configure_file(textures/char_purple.png textures/char_purple.png COPYONLY)
