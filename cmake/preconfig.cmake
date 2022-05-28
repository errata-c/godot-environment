if(NOT TARGET ez::serialize)
	find_dependency(ez-serialize CONFIG)
endif()

if(NOT TARGET godot::cpp)
	find_dependency(godot-cpp CONFIG)
endif()

if(NOT TARGET cppzmq)
	find_dependency(cppzmq CONFIG)
endif()

if(NOT TARGET fmt::fmt)
	find_dependency(fmt CONFIG)
endif()