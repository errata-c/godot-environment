

# EnvironmentNode to interface with Godot
add_library(gdev-environment SHARED
	"src/EnvironmentNode.cpp"
	"src/EnvironmentNodePrint.cpp"
	"src/EnvironmentNodeDefine.cpp"
	"src/gdnlibrary.cpp"
	"src/GodotConversions.cpp"
)
target_link_libraries(gdev-environment PRIVATE
	godot::cpp
	gdev::core
)
target_include_directories(gdev-environment PRIVATE
	"$<INSTALL_INTERFACE:include>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
)
set_property(TARGET gdev-environment PROPERTY OUTPUT_NAME "gdev_environment")
add_library(gdev::environment ALIAS gdev-environment)


install_gdnlib(
	NAME "gdev"
	TARGET "gdev-environment"
	DESTINATION "bin"
	CLASSES "EnvironmentNode"
	COMPONENT GDNLIB
	DEPENDENCIES libzmq
	EXCLUDE_FROM_ALL
)