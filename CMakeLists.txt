cmake_minimum_required(VERSION 3.10)

project("avr_boy_core")



	# ------------------------------------------------------------------------------
	# check if project options are already defined
	if(NOT TARGET project_options)
		add_library(project_options INTERFACE)
		include(cmake/standard-project-settings.cmake)
		target_compile_features(project_options INTERFACE cxx_std_17)
	endif()

if(NOT USE_AVR_GCC)

	# ------------------------------------------------------------------------------
	# add standard compiler warnings
	if(NOT TARGET project_warnings)
		add_library(project_warnings INTERFACE)
		include(cmake/compiler-warnings.cmake)
		set_project_warnings(project_warnings)
	endif()

	# ------------------------------------------------------------------------------
	# setup conan
	set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES} gtest/1.10.0)

	if(NOT CONAN_INITIALIZED)
		set(CONAN_INITIALIZED TRUE)
		include(cmake/conan.cmake)
		run_conan()
	endif()

	add_subdirectory(test)

endif()


add_subdirectory(src)
