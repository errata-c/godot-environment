#include <gdev/EnvironmentNode.hpp>

#include <cassert>
#include <cinttypes>
#include <limits>

#include <gdev/util.hpp>

#include <SceneTree.hpp>
#include <PoolArrays.hpp>
#include <OS.hpp>

namespace godot {
	EnvironmentNode::EnvironmentNode()
		: reward(0.0)
		, mcontext(gdev::Com::Environment)
		, state(State::None)
		, done(false)
	{}
	EnvironmentNode::~EnvironmentNode()
	{}

	gdev::SpaceDef& EnvironmentNode::actionSpaceDef() noexcept {
		return acSpace;
	}
	const gdev::SpaceDef& EnvironmentNode::actionSpaceDef() const noexcept {
		return acSpace;
	}

	gdev::SpaceDef& EnvironmentNode::observationSpaceDef() noexcept {
		return obSpace;
	}
	const gdev::SpaceDef& EnvironmentNode::observationSpaceDef() const noexcept {
		return obSpace;
	}

	bool EnvironmentNode::reset_requested() const noexcept {
		return state == State::Reset;
	}
	bool EnvironmentNode::step_requested() const noexcept {
		return state == State::Step;
	}
	bool EnvironmentNode::send_defs_requested() const noexcept {
		return state == State::SendDefs;
	}
	bool EnvironmentNode::close_requested() const noexcept {
		return state == State::Close;
	}

	void EnvironmentNode::reply() {

	}

	void EnvironmentNode::_register_methods() {
		register_property<EnvironmentNode, double>("reward", &EnvironmentNode::set_reward, &EnvironmentNode::get_reward, 0.0);

		register_method("print_action_space_def", &EnvironmentNode::print_action_space_def);
		register_method("print_observation_space_def", &EnvironmentNode::print_observation_space_def);

		register_method("_ready", &EnvironmentNode::_ready);
		//register_method("_process", &EnvironmentNode::_process);

		register_method("set_action_space", &EnvironmentNode::set_action_space);
		register_method("set_observation_space", &EnvironmentNode::set_observation_space);

		// Signals for the requests:
		register_signal<EnvironmentNode>("send_defs", godot::Dictionary());
		register_signal<EnvironmentNode>("reset", godot::Dictionary());
		register_signal<EnvironmentNode>("step", godot::Dictionary());
		register_signal<EnvironmentNode>("close", godot::Dictionary());
	}

	void EnvironmentNode::_init() {
		// Keep this node running
		set_pause_mode(PAUSE_MODE_PROCESS);
	}

	// Node added to scene tree
	void EnvironmentNode::_ready() {
		//get_tree()->set_pause(true);
		
		// Check for command line args
		godot::PoolStringArray args = godot::OS::get_singleton()->get_cmdline_args();

		int64_t port = 2048;
		bool foundArg = false;

		// Parse args, look for a port number for communication
		for (int i = 0; i < args.size(); ++i) {
			godot::String arg = args[i];
			if (arg.find("=") > -1) {
				// Found an argument that may be a key value pair
				godot::PoolStringArray key_value = arg.split("=");
				if (key_value[0] == "--port") {
					if (key_value[1].is_valid_integer()) {
						port = key_value[1].to_int();
						foundArg = true;
					}
					else {
						Godot::print_error("An invalid integer has been given as a port number!", 
						"EnvironmentNode::_ready", __FILE__, __LINE__);
					}
				}
			}
		}
		
		if (foundArg) {
			Godot::print("Found the port argument!");
		}
		else {
			Godot::print("Failed to find a port argument, defaulting the port to 2048.");
		}
		
		if (mcontext.isConnected()) {
			Godot::print("MessageContext is already connected?");
		}

		// Prep the message context.
		bool result = mcontext.initialize(port);
		if (!result) {
			Godot::print_error("Failed to initialize the MessageContext!",
				"EnvironmentNode::_ready", __FILE__, __LINE__);
		}
		else {
			Godot::print("Successfully initialized the MessageContext!");
		}
		
		// This block was just to check that the connection was working.
		/*
		if (mcontext.checkPing()) {
			Godot::print("Got the ping!");
			if (mcontext.sendPing()) {
				Godot::print("Returned the ping message!");
			}
			else {
				Godot::print("Failed to return the ping message!");
			}
		}
		else {
			Godot::print("Failed to get the ping!");
		}
		*/

		// Pause to make sure the system is synchronized to some extent
		//get_tree()->set_pause(false);
	}

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
		print_space_def(acSpace);
	}
	void EnvironmentNode::print_observation_space_def() {
		print_space_def(obSpace);
	}

	// Should I use this or _physics_process?
	void EnvironmentNode::_process(float delta) {
		// Check for message, block until found
		// Once found, send a signal if appropriate
		// 
	}

	void EnvironmentNode::set_reward(double val) {
		reward = val;
	}
	double EnvironmentNode::get_reward() const noexcept {
		return reward;
	}

	godot::Variant EnvironmentNode::get_action(godot::String name) {
		godot::CharString cname = name.ascii();
		
		auto it = action.find(std::string_view(cname.get_data(), cname.length()));
		if (it == action.end()) {
			Godot::print_error("Failed to find an action value under the requested name!",
				"EnvironmentNode::get_action",
				__FILE__,
				__LINE__);
			return godot::Variant();
		}

		return gdev::convertToVariant(it->value);
	}
	void EnvironmentNode::set_observation(godot::String name, godot::Variant value) {
		godot::CharString cname = name.ascii();

		auto it = observation.find(std::string_view(cname.get_data(), cname.length()));
		if (it == observation.end()) {
			Godot::print_error("Failed to find an observation value under the requested name!", 
			"EnvironmentNode::set_observation",
			__FILE__,
			__LINE__);
			return;
		}

		// Make sure the types are compatible.
		std::optional<gdev::Value> result = gdev::convertToValue(value);
		if (!result) {
			Godot::print_error("Failed to convert the Variant passed into a valid observation value!",
				"EnvironmentNode::set_observation",
				__FILE__,
				__LINE__);
		}
		else {
			it->value = std::move(result.value());
		}
	}

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

	void EnvironmentNode::set_action_space(godot::Dictionary dict) {
		if (fillSpaceDef(dict, "EnvironmentNode::set_action_space", acSpace)) {
			Godot::print("Successfully setup the action space!");
		}
		else {
			Godot::print("Failed to setup the action space!");
		}
	}
	void EnvironmentNode::set_observation_space(godot::Dictionary dict) {
		if (fillSpaceDef(dict, "EnvironmentNode::set_observation_space", obSpace)) {
			Godot::print("Successfully setup the observation space!");
		}
		else {
			Godot::print("Failed to setup the observation space!");
		}
	}
}