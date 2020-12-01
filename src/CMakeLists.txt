# cmake_minimum_required(VERSION 3.10)

add_library(
	${PROJECT_NAME}
	graphx.cpp
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE project_options project_warnings
)

target_include_directories(${PROJECT_NAME} PUBLIC ../include)
