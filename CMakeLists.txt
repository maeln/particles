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

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLEW)
set(CMAKE_CXX_STANDARD 11)

FILE(GLOB_RECURSE HEADERS "src/*.hh")
FILE(GLOB_RECURSE SOURCES "src/*.cc")
FILE(GLOB_RECURSE SHADER_GEOM "data/*.gs")
FILE(GLOB_RECURSE SHADER_VERT "data/*.vs")
FILE(GLOB_RECURSE SHADER_FRAG "data/*.fs")

FILE(GLOB_RECURSE IMGUI_SOURCES "src/imgui/*.cpp")
FILE(GLOB_RECURSE IMGUI_HEADERS "src/imgui/*.h")

source_group("Data" FILES ${SHADER_GEOM} ${SHADER_FRAG} ${SHADER_VERT})
add_executable(
	parts 
	${SOURCES}
	${IMGUI_SOURCES}
	${HEADERS}
	${IMGUI_HEADERS}
	${SHADER_GEOM} 
	${SHADER_FRAG} 
	${SHADER_VERT}
)

target_link_libraries(
	parts
	${ALL_LIBS}
)

set_target_properties(parts PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
