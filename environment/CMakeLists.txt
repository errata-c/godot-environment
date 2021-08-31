

# The static implementation for export
add_library(godot-environment SHARED
	"src/EnvironmentNode.cpp"
	"src/gdnlibrary.cpp"
	"src/util.cpp"
)
target_link_libraries(godot-environment PRIVATE
	godot::cpp
	godot::rlcore
)
target_include_directories(godot-environment PRIVATE
	"$<INSTALL_INTERFACE:include>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
)
set_property(TARGET godot-environment PROPERTY OUTPUT_NAME "gdev")
add_library(godot::environment ALIAS godot-environment)


install_gdnlib(
	NAME "gdev"
	TARGET "godot-environment"
	DESTINATION "bin"
	CLASSES "EnvironmentNode"
	COMPONENT GDNLIB
	TARGET_DEPENDENCIES libzmq
	EXCLUDE_FROM_ALL
)