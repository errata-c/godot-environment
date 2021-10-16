
add_executable(connection_test 
	"connection_test.cpp"
)
target_link_libraries(connection_test PRIVATE
	gdev::core
)