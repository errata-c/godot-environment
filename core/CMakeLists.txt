
add_library(gdev-core STATIC
	"src/ValueType.cpp"
	"src/Value.cpp"
	"src/ValueRef.cpp"
	"src/ValueIterators.cpp"
	
	"src/Space.cpp"
	"src/MessageContext.cpp"
	"src/Serialize.cpp"
)
target_include_directories(gdev-core PUBLIC
	"$<INSTALL_INTERFACE:include>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)
target_compile_features(gdev-core PUBLIC 
	cxx_std_17
)
target_link_libraries(gdev-core PUBLIC 
	libzmq
	ez::serialize
	fmt::fmt
)
target_compile_options(gdev-core PUBLIC
	"$<$<CXX_COMPILER_ID:MSVC>:/wd26812>" # Stop msvc from annoying us with the unscoped enum warning.
)
set_target_properties(gdev-core PROPERTIES EXPORT_NAME core)
add_library(gdev::core ALIAS gdev-core)
