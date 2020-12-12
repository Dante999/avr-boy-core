add_executable(
	unit-tests
	graphx_test.cpp #
	protocol_test.cpp #
	commands_test.cpp
)

target_link_libraries(
	unit-tests
	PRIVATE project_options project_warnings CONAN_PKG::gtest avr_boy_core
)
