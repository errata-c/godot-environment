#include <gdev/EnvironmentNode.hpp>

#include <cassert>
#include <cinttypes>
#include <limits>

#include <gdev/GodotConversions.hpp>
#include <gdev/Serialize.hpp>
#include <ez/deserialize.hpp>

#include <SceneTree.hpp>
#include <Array.hpp>
#include <OS.hpp>
#include <Variant.hpp>
#include <ResourceLoader.hpp>

#include <GridContainer.hpp>
#include <Viewport.hpp>
#include <ViewportContainer.hpp>
#include <World.hpp>
#include <World2D.hpp>
#include <String>

namespace godot {
	EnvironmentNode::Instance::Instance()
		: node(nullptr)
		, done(false)
		, reward(0.f)
	{}
	EnvironmentNode::Instance::Instance(godot::Node* n)
		: node(n)
		, done(false)
		, reward(0.f)
	{}

	EnvironmentNode::EnvironmentNode()
		: mcontext(gdev::Com::Environment)
	{}
	EnvironmentNode::~EnvironmentNode()
	{}

	void EnvironmentNode::_register_methods() {
		register_property<EnvironmentNode, double>("reward", &EnvironmentNode::set_reward, &EnvironmentNode::get_reward, 0.0);
		register_property<EnvironmentNode, bool>("done", &EnvironmentNode::set_done, &EnvironmentNode::get_done, false);

		// Utility for checking that everything was initialized properly
		register_method("print_action_space_def", &EnvironmentNode::print_action_space_def);
		register_method("print_observation_space_def", &EnvironmentNode::print_observation_space_def);

		// General setup:
		register_method("_ready", &EnvironmentNode::_ready);
		// The main process:
		register_method("_physics_process", &EnvironmentNode::_physics_process);

		// For defining the action and observation spaces
		register_method("define_action_space", &EnvironmentNode::define_action_space);
		register_method("define_observation_space", &EnvironmentNode::define_observation_space);

		// For getting and setting actions + observations
		register_method("get_action", &EnvironmentNode::get_action);
		register_method("set_observation", &EnvironmentNode::set_observation);

		register_method("register_scene", &EnvironmentNode::register_scene);
	}

	gdev::SpaceDef& EnvironmentNode::actionSpaceDef() noexcept {
		return maction_space_def;
	}
	const gdev::SpaceDef& EnvironmentNode::actionSpaceDef() const noexcept {
		return maction_space_def;
	}

	gdev::SpaceDef& EnvironmentNode::observationSpaceDef() noexcept {
		return mobservation_space_def;
	}
	const gdev::SpaceDef& EnvironmentNode::observationSpaceDef() const noexcept {
		return mobservation_space_def;
	}

	void EnvironmentNode::set_reward(double val) {
		minstance.reward = val;
	}
	double EnvironmentNode::get_reward() const noexcept {
		return minstance.reward;
	}

	bool EnvironmentNode::get_done() const noexcept {
		return minstance.done;
	}
	void EnvironmentNode::set_done(bool val) noexcept {
		minstance.done = val;
	}
	
	void EnvironmentNode::register_scene(godot::String scene) {
		if (maction_space_def.empty() || mobservation_space_def.empty()) {
			Godot::print_error("Attempt to register a scene with the EnvironmentNode prior to defining the action and observation spaces!",
				"EnvironmentNode::register_scene", __FILE__, __LINE__);
			quit(-1);
			return;
		}

		// Attempt to get the first initialization message

		mbuffer.clear();
		if (mcontext.recv(mbuffer)) {
			gdev::RequestType req = gdev::RequestType::None;
			
			const char* data = mbuffer.data();
			const char* end = mbuffer.data() + mbuffer.size();
			data = gdev::deserialize(data, end, req);
			if (req != gdev::RequestType::Initialize) {
				Godot::print_error("First request received from Agent was not Initialize!",
					"EnvironmentNode::register_scene", __FILE__, __LINE__);
				quit(-1);
				return;
			}
		}
		else {
			Godot::print_error("Failed to get the first message from the Agent!",
				"EnvironmentNode::register_scene", __FILE__, __LINE__);

			quit(-1);
			return;
		}

		mbuffer.clear();
		gdev::serializeDefs(maction_space_def, mobservation_space_def, mbuffer);
		if (!mcontext.send(mbuffer)) {
			Godot::print_error("Failed to send the action and observation space definitions!", 
				"EnvironmentNode::register_scene", __FILE__, __LINE__);

			quit(-1);
			return;
		}

		ResourceLoader * loader = ResourceLoader::get_singleton();
		
		if (!loader->exists(scene)) {
			Godot::print_error("Scene path passed into EnvironmentNode::register_scene does not exist!",
				"EnvironmentNode::register_scene", __FILE__, __LINE__);
			quit(-1);
			return;
		}

		Ref<PackedScene> packed = loader->load(scene);
		if (packed.is_null()) {
			godot::String errorMsg = godot::String("Path '{0}' did not refer to a PackedScene resource!").format(scene);
			Godot::print_error(errorMsg,
				"EnvironmentNode::register_scene", __FILE__, __LINE__);
			quit(-1);
			return;
		}
		if (!packed->can_instance()) {
			godot::String errorMsg = godot::String("PackedScene '{0}' does not contain any nodes!").format(scene);
			Godot::print_error(errorMsg,
				"EnvironmentNode::register_scene", __FILE__, __LINE__);
			quit(-1);
			return;
		}

		prepareInstance(packed);
	}

	void EnvironmentNode::prepareInstance(const Ref<PackedScene>& packed) {
		godot::Node* node = packed->instance();

		// We need to first check that the node follows the expected interface, but only once
		if (!node->has_method("step")) {
			godot::String errorMsg = godot::String("Node '{0}' does not have a 'step' method!").format(node->get_name());
			Godot::print_error(errorMsg,
				"EnvironmentNode::register_scene", __FILE__, __LINE__);
			quit(-1);
			return;
		}
		if (!node->has_method("reset")) {
			godot::String errorMsg = godot::String("Node '{0}' does not have a 'reset' method!").format(node->get_name());
			Godot::print_error(errorMsg,
				"EnvironmentNode::register_scene", __FILE__, __LINE__);
			quit(-1);
			return;
		}
			
			
		add_child(node);
		

		minstance.node = node;
		minstance.action = maction_space_def.instance();
		minstance.observation = mobservation_space_def.instance();
		minstance.done = false;
		minstance.reward = 0.0;
	}

	void EnvironmentNode::_init() {
		// Keep this node running
		set_pause_mode(PAUSE_MODE_PROCESS);
	}

	// Node added to scene tree
	void EnvironmentNode::_ready() {
		// Check for command line args
		godot::PoolStringArray args = godot::OS::get_singleton()->get_cmdline_args();

		int64_t port = 2048;
		bool foundPort = false;

		// Parse args, look for a port number for communication
		for (int i = 0; i < args.size(); ++i) {
			godot::String arg = args[i];
			if (arg.find("=") > -1) {
				// Found an argument that may be a key value pair
				godot::PoolStringArray key_value = arg.split("=");
				if (key_value[0] == "--port") {
					port = key_value[1].to_int();
					foundPort = true;
				}
				else if (key_value[0] == "--recv_timeout") {
					int ms = key_value[1].to_int();
					mcontext.setRecvTimeout(ms);
					Godot::print(godot::String("Set the recv timeout to {0}").format(godot::Array::make(ms)));
				}
				else if (key_value[0] == "--send_timeout") {
					int ms = key_value[1].to_int();
					mcontext.setSendTimeout(ms);
					Godot::print(godot::String("Set the send timeout to {0}").format(godot::Array::make(ms)));
				}
				else if (key_value[0] == "--connect_timeout") {
					int ms = key_value[1].to_int();
					mcontext.setConnectTimeout(ms);
					Godot::print(godot::String("Set the connect timeout to {0}").format(godot::Array::make(ms)));
				}
			}
		}
		
		if (foundPort) {
			Godot::print("Found the port argument!");
		}
		else {
			Godot::print("Failed to find a port argument, defaulting the port to 2048.");
		}

		// Prep the message context.
		bool result = mcontext.initialize(port);
		if (!result) {
			Godot::print_error("Failed to initialize the MessageContext!",
				"EnvironmentNode::_ready", __FILE__, __LINE__);
			quit(-1);
			return;
		}
		else {
			Godot::print("Successfully initialized the MessageContext!");
		}
	}

	void EnvironmentNode::quit(int code) {
		SceneTree* tree = get_tree();
		tree->quit(code);
	}
	
	void EnvironmentNode::_physics_process(float delta) {
		gdev::RequestType req = gdev::RequestType::None;

		mbuffer.clear();
		if (mcontext.recv(mbuffer)) {
			if (mbuffer.empty()) {
				Godot::print_error("Failed to recv a message from the agent!", "EnvironmentNode::_physics_process", __FILE__, __LINE__);
				quit(-1);
				return;
			}

			const char* data = mbuffer.data();
			const char* end = mbuffer.data() + mbuffer.size();
			data = gdev::deserialize(data, end, req);

			switch (req) {
			default: {
				godot::String errorMsg = "Received an invalid value of '{0}' for the request type!";
				Godot::print_error(errorMsg.format(godot::Array::make(static_cast<int>(req))), "EnvironmentNode::reply", __FILE__, __LINE__);
				quit(-1);
				break;
			}
			case gdev::RequestType::Close: {
				Godot::print("Received close command, ending the training session!");
				quit(0);
				break;
			}
			case gdev::RequestType::Reset: {
				// No data in request
				int32_t index;
				data = ez::deserialize::i32(data, end, index);

				minstance.node->call("reset", godot::Array::make(this));

				mbuffer.clear();
				gdev::serializeStep(minstance.observation, minstance.reward, minstance.done, mbuffer);
				if (!mcontext.send(mbuffer)) {
					Godot::print_error("Failed to send the step data!", "EnvironmentNode::reply", __FILE__, __LINE__);
					quit(-1);
					return;
				}
				break;
			}
			case gdev::RequestType::Step: {
				// Step contains the action data
				int32_t index;
				data = ez::deserialize::i32(data, end, index);

				// Deserialize the action data!
				data = gdev::deserialize(data, end, minstance.action);
				minstance.node->call("step", godot::Array::make(this));

				mbuffer.clear();
				gdev::serializeStep(minstance.observation, minstance.reward, minstance.done, mbuffer);
				if (!mcontext.send(mbuffer)) {
					Godot::print_error("Failed to send the step data!", "EnvironmentNode::reply", __FILE__, __LINE__);
					quit(-1);
					return;
				}
				break;
			}}
		}
		else {
			Godot::print_error("Failed to recv a message from the agent!", "EnvironmentNode::_physics_process", __FILE__, __LINE__);
			quit(-1);
			return;
		}
		
	}

	godot::Variant EnvironmentNode::get_action(godot::String name) {
		godot::CharString cname = name.ascii();

		gdev::Space & action = minstance.action;
		
		auto it = action.find(std::string_view(cname.get_data(), cname.length()));
		if (it == action.end()) {
			godot::String msg{"Failed to find an action value under the name '{0}'!"};
			Godot::print_error(msg.format(godot::Array::make(name)),
				"EnvironmentNode::get_action",
				__FILE__,
				__LINE__);
			return godot::Variant();
		}

		return gdev::convertToVariant(it->value);
	}
	void EnvironmentNode::set_observation(godot::String name, godot::Variant value) {
		godot::CharString cname = name.ascii();

		gdev::Space & observation = minstance.observation;

		auto it = observation.find(std::string_view(cname.get_data(), cname.length()));
		if (it == observation.end()) {
			godot::String msg{ "Failed to find an observation value under the name '{0}'!" };
			Godot::print_error(msg.format(godot::Array::make(name)), 
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
}