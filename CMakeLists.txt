cmake_minimum_required(VERSION 3.19)

project(GODOT_ENVIRONMENT
LANGUAGES C CXX
DESCRIPTION "A framework for building reinforcement learning environments using the Godot game engine."
)

include(GNUInstallDirs)
include(FetchContent)

FetchContent_Declare(
	ez-cmake
	GIT_REPOSITORY "https://github.com/errata-c/ez-cmake.git"
	GIT_TAG "main"
)
FetchContent_MakeAvailable(ez-cmake)

find_package(ez-serialize CONFIG REQUIRED)
find_package(godot-cpp CONFIG REQUIRED)
find_package(cppzmq CONFIG REQUIRED)

# create the core library, shared features of both the agent and environment.
# adds godot::rlcore target
add_subdirectory("core")

# Create the RL agent library, contains the necessary code to communicate with the environment.
# adds godot::agent target
add_subdirectory("agent")

# Create the RL environment library, contains the necessary integration with the godot game engine.
# adds godot::environment target
add_subdirectory("environment")


# Installing the agent library should not drag the environment library with it.
# They must be separate components.

# Provide a component for installing the environment library as a final product to Godot.
# Provide another component for installing the environment library as an export.
# Provide a component for installing the agent library as an export.