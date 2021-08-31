
add_library(godot-rlcore STATIC
	"src/Value.cpp"
	"src/ValueRef.cpp"
	"src/ValueIterators.cpp"

	"src/ValueDef.cpp"
	"src/SpaceDef.cpp"
	"src/Space.cpp"
	"src/MessageContext.cpp"
	"src/Serialize.cpp"
)
target_include_directories(godot-rlcore PUBLIC
	"$<INSTALL_INTERFACE:include>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)
target_compile_features(godot-rlcore PUBLIC 
	cxx_std_17
)
target_link_libraries(godot-rlcore PUBLIC 
	cppzmq
	ez::serialize
)

add_library(godot::rlcore ALIAS godot-rlcore)

add_subdirectory("test")