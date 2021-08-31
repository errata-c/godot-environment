
add_library(godot-agent STATIC 
	"src/Environment.cpp"
)
target_link_libraries(godot-agent PUBLIC 
	godot::rlcore
)
target_include_directories(godot-agent PUBLIC 
	"$<INSTALL_INTERFACE:include>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
)

add_subdirectory("test")
