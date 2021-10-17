
add_library(gdev-agent STATIC 
	"src/Agent.cpp"
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

add_subdirectory("test")
