
include(Catch)

add_executable(gdev_core_test 
	"value/init.cpp"
	"value/move.cpp"
	"value/access.cpp"

	"space.cpp"
	"serialization.cpp"
	"communication.cpp"
)
target_link_libraries(gdev_core_test PRIVATE
	Catch2::Catch2WithMain
	gdev::core
)