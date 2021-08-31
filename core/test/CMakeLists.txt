find_package(Catch2 CONFIG REQUIRED)

add_executable(rlcore_test 
	"main.cpp"
	"value.cpp"
	"value_def.cpp"
	"space.cpp"
	"space_def.cpp"
	"serialization.cpp"
)
target_link_libraries(rlcore_test PRIVATE
	Catch2::Catch2
	godot::rlcore
)