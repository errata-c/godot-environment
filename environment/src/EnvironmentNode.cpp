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
		, currentIndex(0)
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

	void EnvironmentNode::set_reward(double val) {
		instances[currentIndex].reward = val;
	}
	double EnvironmentNode::get_reward() const noexcept {
		return instances[currentIndex].reward;
	}

	bool EnvironmentNode::get_done() const noexcept {
		return instances[currentIndex].done;
	}
	void EnvironmentNode::set_done(bool val) noexcept {
		instances[currentIndex].done = val;
	}
	
	void EnvironmentNode::register_scene(godot::String scene) {
		if (acSpace.empty() || obSpace.empty()) {
			Godot::print_error("Attempt to register a scene with the EnvironmentNode prior to defining the action and observation spaces!",
				"EnvironmentNode::register_scene", __FILE__, __LINE__);
			quit(-1);
			return;
		}

		// Attempt to get the first initialization message
		int numInstances = 0;

		buffer.clear();
		if (mcontext.recv(buffer)) {
			gdev::RequestType req = gdev::RequestType::None;
			
			const uint8_t* data = buffer.data();
			const uint8_t* end = buffer.data() + buffer.size();
			data = gdev::deserialize(data, end, req);
			if (req != gdev::RequestType::Initialize) {
				Godot::print_error("First request received from Agent was not Initialize!",
					"EnvironmentNode::register_scene", __FILE__, __LINE__);
				quit(-1);
				return;
			}
			
			data = ez::deserialize::i32(data, end, numInstances);
		}
		else {
			Godot::print_error("Failed to get the first message from the Agent!",
				"EnvironmentNode::register_scene", __FILE__, __LINE__);

			quit(-1);
			return;
		}

		buffer.clear();
		gdev::serializeDefs(acSpace, obSpace, buffer);
		if (!mcontext.send(buffer)) {
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

		prepareInstances(numInstances, packed);
	}

	void EnvironmentNode::prepareInstances(int numInstances, const Ref<PackedScene>& packed) {
		// Determine the number of columns to use when splitting the screen.
		int columns = std::sqrt(numInstances);
		Vector2 scaleFactor;
		{
			float factor = 1.f / float(columns);
			scaleFactor = Vector2(factor, factor);
		}
		
		// We create a grid container to organize the viewports
		GridContainer * grid = GridContainer::_new();
		grid->set_columns(columns);
		grid->set_anchors_preset(Control::PRESET_WIDE);
		add_child(grid);

		for (int i = 0; i < numInstances; ++i) {
			ViewportContainer * viewBox = ViewportContainer::_new();
			Viewport* view = Viewport::_new();
			viewBox->set_stretch(true);
			viewBox->set_h_size_flags(viewBox->get_h_size_flags() | Control::SIZE_EXPAND);
			viewBox->set_v_size_flags(viewBox->get_v_size_flags() | Control::SIZE_EXPAND);

			// Each viewport needs its own world to prevent conflicts and interaction between the instances
			view->set_world(Ref<World>(World::_new()));
			view->set_world_2d(Ref<World2D>(World2D::_new()));
			view->set_use_own_world(true);
			view->set_global_canvas_transform(view->get_global_canvas_transform().scaled(scaleFactor));

			grid->add_child(viewBox);
			viewBox->add_child(view);

			godot::Node* node = packed->instance();

			// We need to first check that the node follows the expected interface, but only once
			if (i == 0) {
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
			}
			
			view->add_child(node);
			instances.emplace_back(node);
		}

		for (auto & inst : instances) {
			inst.observation = observationSpaceDef().instance();
		}
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
						quit(-1);
						return;
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
		// Get messages for each of the instances.

		// There may be a better way to determine when to end the message loop here.
		// The main issue is the possibility of 
		int stepCount = 0;

		gdev::RequestType req = gdev::RequestType::None;
		while (stepCount != instances.size()) {
			buffer.clear();
			if (mcontext.recv(buffer)) {
				if (buffer.empty()) {
					Godot::print_error("Failed to recv a message from the agent!", "EnvironmentNode::_physics_process", __FILE__, __LINE__);
					quit(-1);
					return;
				}

				const uint8_t* data = buffer.data();
				const uint8_t* end = buffer.data() + buffer.size();
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
					currentIndex = index;

					Instance& inst = instances[index];
					inst.node->call("reset", godot::Array::make(this));

					buffer.clear();
					gdev::serializeStep(inst.observation, inst.reward, inst.done, buffer);
					if (!mcontext.send(buffer)) {
						Godot::print_error("Failed to send the step data!", "EnvironmentNode::reply", __FILE__, __LINE__);
						quit(-1);
						return;
					}

					++stepCount;
					break;
				}
				case gdev::RequestType::Step: {
					// Step contains the action data
					int32_t index;
					data = ez::deserialize::i32(data, end, index);
					currentIndex = index;

					Instance & inst = instances[index];

					// Deserialize the action data!
					data = gdev::deserialize(data, end, inst.action);
					inst.node->call("step", godot::Array::make(this));

					buffer.clear();
					gdev::serializeStep(inst.observation, inst.reward, inst.done, buffer);
					if (!mcontext.send(buffer)) {
						Godot::print_error("Failed to send the step data!", "EnvironmentNode::reply", __FILE__, __LINE__);
						quit(-1);
						return;
					}

					++stepCount;
					break;
				}}
			}
			else {
				Godot::print_error("Failed to recv a message from the agent!", "EnvironmentNode::_physics_process", __FILE__, __LINE__);
				quit(-1);
				return;
			}
		}
	}

	godot::Variant EnvironmentNode::get_action(godot::String name) {
		godot::CharString cname = name.ascii();

		gdev::Space & action = instances[currentIndex].action;
		
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

		gdev::Space & observation = instances[currentIndex].observation;

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