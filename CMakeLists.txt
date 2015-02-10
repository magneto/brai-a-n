cmake_minimum_required(VERSION 2.4)

project(braian)

file(
	GLOB_RECURSE
	braian_sources
	sources/*.cpp
)

set(
	CMAKE_C_FLAGS
	"/clr /TP"
)

set(
	CMAKE_CXX_FLAGS
	"/clr /TP"
)

set(
	CMAKE_CXX_FLAGS_DEBUG
	"/clr /TP /DDEBUG"
)

add_executable(
	${CMAKE_PROJECT_NAME}
	${braian_sources}
)
