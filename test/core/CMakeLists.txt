
include(Catch)

add_executable(gdev_core_test 
	"value.cpp"
	"value_def.cpp"
	"space.cpp"
	"space_def.cpp"
	"serialization.cpp"
)
target_link_libraries(gdev_core_test PRIVATE
	Catch2::Catch2WithMain
	gdev::core
)