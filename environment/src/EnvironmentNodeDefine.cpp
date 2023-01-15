#include <gdev/EnvironmentNode.hpp>

#include <Godot.hpp>
#include <GodotGlobal.hpp>
#include <gdev/GodotConversions.hpp>
#include <gdev/ValueWrapper.hpp>

namespace godot {
	bool fillSpaceDef(godot::Dictionary dict, godot::String funcName, godot::Dictionary& space) {
		using VType = godot::Variant::Type;

		// Iterate all the key value pairs in the dictionary
		// Add each one as a component of the space
		godot::Array keys = dict.keys();
		godot::Array values = dict.values();
		assert(keys.size() == values.size());

		for (std::size_t i = 0, c = values.size(); i < c; ++i) {
			if (keys[i].get_type() != VType::STRING) {
				Godot::print_error("Non-string key values are not supported for spaces!",
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

			gdev::ValueType vtype;
			gdev::range_t range;
			gdev::Dims dims{ 1,1,1,1 };

			if (!value.has("type")) {
				ERR_PRINT(
					String("Space element '{0}' does not have a type defined!").format(Array::make(key))
				);
				return false;
			}
			else {
				auto opt = gdev::to_value_type(value["type"]);
				if (!opt) {
					return false;
				}
				vtype = *opt;
			}

			if (!value.has("range")) {
				ERR_PRINT(
					String("Space element '{0}' does not have a range defined!").format(Array::make(key))
				);
				return false;
			}
			else {
				auto opt = gdev::to_range(value["range"]);
				if (!opt) {
					return false;
				}
				range = *opt;
			}

			if (value.has("dims")) {
				auto opt = gdev::to_dims(value["dims"]);
				if (!opt) {
					ERR_PRINT(
						String("Failed to process '{0}' dims, invalid formatting!").format(Array::make(key))
					);
					return false;
				}
				dims = *opt;
			}

			ValueWrapper * wrapped = ValueWrapper::_new();
			// Range?
			wrapped->data = gdev::Value::Uninitialized(dims, vtype);

			space[key] = wrapped;
		}

		return true;
	}

	void EnvironmentNode::define_action_space(godot::Dictionary dict) {
		if (fillSpaceDef(dict, "EnvironmentNode::define_action_space", maction_space)) {
			Godot::print("Successfully defined the action space!");
		}
		else {
			Godot::print("Failed to define the action space!");
		}
	}
	void EnvironmentNode::define_observation_space(godot::Dictionary dict) {
		if (fillSpaceDef(dict, "EnvironmentNode::define_observation_space", mobservation_space)) {
			Godot::print("Successfully defined the observation space!");
		}
		else {
			Godot::print("Failed to define the observation space!");
		}
	}
}