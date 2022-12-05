
add_executable(exec_test
	"main.cpp"
)
target_link_libraries(exec_test PRIVATE 
	gdev::agent
)
target_include_directories(exec_test PRIVATE
	"${CONFIG_DIRECTORY}"
)