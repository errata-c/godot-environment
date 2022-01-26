
add_library(gdev-agent STATIC 
	"src/Agent.cpp"
	"src/Step.cpp"
	"src/platform_independent.cpp"
	"$<$<PLATFORM_ID:Windows>:src/windows.cpp>"
	"$<$<PLATFORM_ID:Linux>:src/linux.cpp>"
)
target_link_libraries(gdev-agent PUBLIC 
	gdev::core
)
target_include_directories(gdev-agent PUBLIC 
	"$<INSTALL_INTERFACE:include>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
)
add_library(gdev::agent ALIAS gdev-agent)
set_target_properties(gdev-agent PROPERTIES EXPORT_NAME agent)

if(GODOT_ENVIRONMENT_BUILD_TESTS)
	add_subdirectory("test")
endif()