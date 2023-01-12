#include <Godot.hpp>
#include <gdev/EnvironmentNode.hpp>
#include <gdev/ValueWrapper.hpp>
#include <gdev/ValueIndexer.hpp>

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options * o) {
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options * o) {
    godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void* handle) {
    godot::Godot::nativescript_init(handle);

	godot::register_class<godot::ValueWrapper>();
	godot::register_class<godot::ValueIndexer>();
    godot::register_class<godot::EnvironmentNode>();
}