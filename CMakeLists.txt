cmake_minimum_required(VERSION 3.10)

project("avr_boy_core")

# ------------------------------------------------------------------------------
# check if project options are already defined
if(NOT TARGET project_options)
	add_library(project_options INTERFACE)
	target_compile_features(project_options INTERFACE cxx_std_17)
endif()

# ------------------------------------------------------------------------------
# add standard compiler warnings
if(NOT TARGET project_warnings)
	add_library(project_warnings INTERFACE)
	include(cmake/compiler-warnings.cmake)
	set_project_warnings(project_warnings)
endif()

add_subdirectory(src)
