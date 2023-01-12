
# Find the scene file, configure a file for it.
set(GDEV_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pendulum_environment")
configure_file("config.hpp.in" "${CMAKE_CURRENT_BINARY_DIR}/evtest/config.hpp" @ONLY)
set(CONFIG_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/evtest)

# Test for the Agent class using a simple pendulum environment
# Adds the pendulum_agent_test target
add_subdirectory("pendulum_agent")

# Test for the exec function implementations
# Adds the exec_test target
add_subdirectory("exec")

