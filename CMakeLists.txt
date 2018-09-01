cmake_minimum_required (VERSION 3.8)
project (parts)

find_package(OpenGL REQUIRED)

add_subdirectory (external)

include_directories(
	external/glfw-3.2.1/include/GLFW/
	external/glm-0.9.8.5/glm/
	external/glew-2.1.0/include/
	external/stb_image/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_2100
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

FILE(GLOB_RECURSE HEADERS "src/*.hh")
FILE(GLOB_RECURSE SHADER_GEOM "data/*.gs")
FILE(GLOB_RECURSE SHADER_VERT "data/*.vs")
FILE(GLOB_RECURSE SHADER_FRAG "data/*.fs")
source_group("Data" FILES ${SHADER_GEOM} ${SHADER_FRAG} ${SHADER_VERT})
add_executable(
	parts 
	src/main.cc 
	src/tools.cc 
	src/shader_handler.cc 
	src/window_handler.cc 
	src/particle_handler.cc 
	src/camera.cc 
	src/indiced_mesh.cc 
	src/basic_mesh.cc 
	${HEADERS}
	${SHADER_GEOM} 
	${SHADER_FRAG} 
	${SHADER_VERT}
)

target_link_libraries(
	parts
	${ALL_LIBS}
)

set_target_properties(parts PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
