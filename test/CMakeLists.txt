cmake_minimum_required(VERSION 3.24)

if(NOT TARGET "gdev::core")
	message(FATAL_ERROR "The target 'gdev::core' was not found! This target is required for testing.")
endif()
if(NOT TARGET "gdev::agent")
	message(FATAL_ERROR "The target 'gdev::agent' was not found! This target is required for testing.")
endif()

# The Catch2 unit testing framework is used for testing here
FetchContent_Declare(
	Catch2
	GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
	GIT_TAG "v3.2.1"
	FIND_PACKAGE_ARGS CONFIG REQUIRED
)

FetchContent_MakeAvailable(Catch2)

add_subdirectory("agent")
add_subdirectory("core")