#include <gdev/EnvironmentNode.hpp>

#include <Godot.hpp>
#include <GodotGlobal.hpp>
#include <gdev/GodotConversions.hpp>

namespace godot {
	bool fillSpaceDef(godot::Dictionary dict, godot::String funcName, gdev::SpaceDef& space) {
		using VType = godot::Variant::Type;

		// Iterate all the key value pairs in the dictionary
		// Add each one as a component of the space
		godot::Array keys = dict.keys();
		godot::Array values = dict.values();
		assert(keys.size() == values.size());

		for (std::size_t i = 0; i < values.size(); ++i) {
			if (keys[i].get_type() != VType::STRING) {
				Godot::print_error("Non-string key values are not supported for action or observation space!",
					funcName, __FILE__, __LINE__);

				space.clear();
				return false;
			}
			if (values[i].get_type() != VType::DICTIONARY) {
				Godot::print_warning("Setting up the action or observation space requires a dictionary of dictionaries",
					funcName, __FILE__, __LINE__);
				continue;
			}

			godot::String key = keys[i];
			godot::Dictionary value = values[i];

			std::optional<gdev::ValueDef> vdef = gdev::convertToValueDef(value);
			if (!vdef) {
				Godot::print_error("Failed to convert a dictionary value into a ValueDef!",
					funcName, __FILE__, __LINE__);

				space.clear();
				return false;
			}

			godot::CharString name = key.ascii();
			std::string_view nameView(name.get_data(), name.length());

			auto res = space.insert(nameView, std::move(vdef.value()));
			assert(res.has_value());
		}

		return true;
	}

	void EnvironmentNode::define_action_space(godot::Dictionary dict) {
		if (acSpace.empty()) {
			if (fillSpaceDef(dict, "EnvironmentNode::define_action_space", acSpace)) {
				Godot::print("Successfully defined the action space!");
			}
			else {
				Godot::print("Failed to define the action space!");
			}
		}
		else {
			Godot::print_error("Action space definition has already been initialized!", "EnvironmentNode::define_action_space", __FILE__, __LINE__);
		}
	}
	void EnvironmentNode::define_observation_space(godot::Dictionary dict) {
		if (obSpace.empty()) {
			if (fillSpaceDef(dict, "EnvironmentNode::define_observation_space", obSpace)) {
				Godot::print("Successfully defined the observation space!");
			}
			else {
				Godot::print("Failed to define the observation space!");
			}
		}
		else {
			Godot::print_error("Observation space definition has already been initialized!", "EnvironmentNode::define_observation_space", __FILE__, __LINE__);
		}
	}
}