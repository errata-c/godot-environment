
add_executable(pendulum_agent_test 
	"main.cpp"
)
target_link_libraries(pendulum_agent_test PRIVATE
	gdev::agent
)
target_include_directories(pendulum_agent_test PRIVATE
	"${CONFIG_DIRECTORY}"
)