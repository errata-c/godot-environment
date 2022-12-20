#include <gdev/EnvironmentNode.hpp>

#include <Godot.hpp>
#include <GodotGlobal.hpp>

namespace godot {
	static void print_space_def(const gdev::SpaceDef& space) {
		// Print the name of the entry, the type, the size, and the range.
		for (const auto& entry : space) {
			godot::String name = entry.name.c_str();
			godot::String type;

			switch (entry.value.type()) {
			default:
			case gdev::ValueType::Bool:
				type = "Bool";
				break;
			case gdev::ValueType::Int:
				type = "Int";
				break;
			case gdev::ValueType::Real:
				type = "Real";
				break;
			}

			Godot::print("name: '{0}', type: {1}, range: [{2}, {3}], dims: [{4}, {5}, {6}, {7}]",
				name,
				type,
				entry.value.lowerBound(),
				entry.value.upperBound(),
				entry.value.dim(0),
				entry.value.dim(1),
				entry.value.dim(2),
				entry.value.dim(3)
			);
		}
	}

	void EnvironmentNode::print_action_space_def() {
		print_space_def(maction_space_def);
	}
	void EnvironmentNode::print_observation_space_def() {
		print_space_def(mobservation_space_def);
	}
}