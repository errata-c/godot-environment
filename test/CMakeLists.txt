
project(GODOT_ENVIRONMENT_TESTS
	LANGUAGES C CXX
	VERSION 0.1.0
	DESCRIPTION "Testing for godot-environment and its components"
)

if(NOT TARGET "gdev::core")
	message(FATAL_ERROR "The target 'gdev::core' was not found! This target is required for testing.")
endif()
if(NOT TARGET "gdev::agent")
	message(FATAL_ERROR "The target 'gdev::agent' was not found! This target is required for testing.")
endif()

# The Catch2 unit testing framework is used for testing here
find_package(Catch2 CONFIG REQUIRED)

add_subdirectory("agent")
add_subdirectory("core")