cmake_minimum_required(VERSION 3.24)

project(GODOT_ENVIRONMENT
	LANGUAGES C CXX
	VERSION 0.2.0
	DESCRIPTION "A framework for building reinforcement learning environments using the Godot game engine."
)

set(GODOT_ENVIRONMENT_CONFIG_DIR "share/godot-environment" CACHE STRING "The relative destination of the configuration files")

include(GNUInstallDirs)
include(FetchContent)

# Used for installation of this project as a package
FetchContent_Declare(
	ez-cmake
	GIT_REPOSITORY "https://github.com/errata-c/ez-cmake.git"
	GIT_TAG "main"
)
# Used for serialization of data passed between agent and environment.
FetchContent_Declare(
	ez-serialize
	GIT_REPOSITORY "https://github.com/errata-c/ez-serialize.git"
	GIT_TAG "main"
	FIND_PACKAGE_ARGS CONFIG REQUIRED
)
# Used to interface with the godot game engine. Major version 3 required.
FetchContent_Declare(
	godot-cpp
	GIT_REPOSITORY "https://github.com/errata-c/godot-cpp.git"
	GIT_TAG "3.x"
	FIND_PACKAGE_ARGS 3 CONFIG REQUIRED
)
# Used for formatting
FetchContent_Declare(
	fmt
	GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
	GIT_TAG "9.1.0"
	FIND_PACKAGE_ARGS CONFIG REQUIRED
)

FetchContent_MakeAvailable(ez-cmake ez-serialize godot-cpp fmt)


# Used to facilitate the request reply loop between the agent and the environment.
find_package(ZeroMQ CONFIG REQUIRED)


# create the core library, shared features of both the agent and environment.
# adds gdev::core target
add_subdirectory("core")

# Create the RL agent library, contains the necessary code to communicate with the environment.
# adds gdev::agent target
add_subdirectory("agent")

# Create the RL environment library, contains the necessary integration with the godot game engine.
# adds gdev::environment target
add_subdirectory("environment")

if(PROJECT_IS_TOP_LEVEL)
	include(CTest)
	if(BUILD_TESTING)
		add_subdirectory("test")
	endif()

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
endif()