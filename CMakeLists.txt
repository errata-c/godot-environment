cmake_minimum_required(VERSION 3.19)

project(GODOT_ENVIRONMENT
	LANGUAGES C CXX
	VERSION 0.2.0
	DESCRIPTION "A framework for building reinforcement learning environments using the Godot game engine."
)

option(GODOT_ENVIRONMENT_BUILD_TESTS "Build the tests for godot-environment" OFF)
set(GODOT_ENVIRONMENT_CONFIG_DIR "share/godot-environment" CACHE STRING "The relative destination of the configuration files")

include(GNUInstallDirs)
include(FetchContent)

FetchContent_Declare(
	ez-cmake
	GIT_REPOSITORY "https://github.com/errata-c/ez-cmake.git"
	GIT_TAG "main"
)
FetchContent_MakeAvailable(ez-cmake)

# Used for serialization of data passed between agent and environment.
find_package(ez-serialize CONFIG REQUIRED)

# Used to interface with the godot game engine. Major version 3 required.
find_package(godot-cpp 3 CONFIG REQUIRED)

# Used to facilitate the request reply loop between the agent and the environment.
find_package(cppzmq CONFIG REQUIRED)

# create the core library, shared features of both the agent and environment.
# adds gdev::core target
add_subdirectory("core")

# Create the RL agent library, contains the necessary code to communicate with the environment.
# adds gdev::agent target
add_subdirectory("agent")

# Create the RL environment library, contains the necessary integration with the godot game engine.
# adds gdev::environment target
add_subdirectory("environment")


# Install all the include files
install(DIRECTORY "core/include/" DESTINATION "include"
	FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
install(DIRECTORY "agent/include/" DESTINATION "include"
	FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

install(TARGETS gdev-core gdev-agent
	EXPORT gdev-export
	RUNTIME DESTINATION "bin/$<CONFIG>"
	ARCHIVE DESTINATION "lib/$<CONFIG>"
	LIBRARY DESTINATION "lib/$<CONFIG>"
)

install_package(
	NAME "godot-environment"
	NAMESPACE "gdev::"
	EXPORT "gdev-export"
	VERSION "${GODOT_ENVIRONMENT_VERSION}"
	COMPATIBILITY "SameMajorVersion"
	DESTINATION "${GODOT_ENVIRONMENT_CONFIG_DIR}"
	PRECONFIG "cmake/preconfig.cmake"
)