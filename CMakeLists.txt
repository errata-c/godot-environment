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

# Used for serialization of data passed between agent and environment.
find_package(ez-serialize CONFIG REQUIRED)

# Used to interface with the godot game engine.
find_package(godot-cpp 4 CONFIG REQUIRED)

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