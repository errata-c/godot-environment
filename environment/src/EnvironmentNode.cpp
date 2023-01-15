#include <gdev/EnvironmentNode.hpp>

#include <cassert>
#include <cinttypes>
#include <limits>

#include <gdev/GodotConversions.hpp>
#include <gdev/Serialize.hpp>
#include <ez/serialize/core.hpp>

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
	EnvironmentNode::EnvironmentNode()
		: mcontext(gdev::Com::Environment)
		, mdone(false)
		, muse_physics_process(false)
		, mhuman_control(false)
		, mreward(0.0)
		, mscene(nullptr)
	{}
	EnvironmentNode::~EnvironmentNode()
	{}

	void EnvironmentNode::_register_methods() {
		register_property<EnvironmentNode, bool>("use_physics_process", &EnvironmentNode::muse_physics_process, false);
		register_property<EnvironmentNode, bool>("human_control", &EnvironmentNode::mhuman_control, false);
		register_property<EnvironmentNode, godot::Dictionary>("action_space", &EnvironmentNode::maction_space, godot::Dictionary{});
		register_property<EnvironmentNode, godot::Dictionary>("observation_space", &EnvironmentNode::mobservation_space, godot::Dictionary{});
		register_property<EnvironmentNode, double>("reward", &EnvironmentNode::set_reward, &EnvironmentNode::get_reward, 0.0);
		register_property<EnvironmentNode, bool>("done", &EnvironmentNode::set_done, &EnvironmentNode::get_done, false);

		// General setup:
		register_method("_ready", &EnvironmentNode::_ready);

		// The main process:
		register_method("_process", &EnvironmentNode::_process);
		register_method("_physics_process", &EnvironmentNode::_physics_process);

		// For defining the action and observation spaces
		register_method("define_action_space", &EnvironmentNode::define_action_space);
		register_method("define_observation_space", &EnvironmentNode::define_observation_space);

		register_method("register_scene", &EnvironmentNode::register_scene);
		register_method("set_connect_timeout", &EnvironmentNode::set_connect_timeout);
		register_method("set_send_timeout", &EnvironmentNode::set_send_timeout);
		register_method("set_recv_timeout", &EnvironmentNode::set_recv_timeout);
		register_method("connect_agent", &EnvironmentNode::connect_agent);

		register_method("setup", &EnvironmentNode::setup);
	}

	void EnvironmentNode::set_reward(double val) {
		mreward = val;
	}
	double EnvironmentNode::get_reward() const noexcept {
		return mreward;
	}

	bool EnvironmentNode::get_done() const noexcept {
		return mdone;
	}
	void EnvironmentNode::set_done(bool val) noexcept {
		mdone = val;
	}
	
	void EnvironmentNode::set_connect_timeout(int ms) {
		mcontext.setConnectTimeout(ms);
	}
	void EnvironmentNode::set_send_timeout(int ms) {
		mcontext.setSendTimeout(ms);
	}
	void EnvironmentNode::set_recv_timeout(int ms) {
		mcontext.setRecvTimeout(ms);
	}
	bool EnvironmentNode::connect_agent(int port) {
		if (!mcontext.connect(port)) {
			ERR_PRINT("Failed to connect to the agent!");
			return false;
		}
		else {
			Godot::print("Connection initialized");
			return true;
		}
	}

	void EnvironmentNode::register_scene(godot::Node * node) {
		if (node == nullptr) {
			Godot::print_error("Attempt to register a scene with a null Node!",
				"EnvironmentNode::register_scene", __FILE__, __LINE__);
			quit(-1);
			return;
		}

		// First make sure the node has the required methods:
		if (!node->has_method("step")) {
			godot::String errorMsg = godot::String("Node '{0}' does not have the required 'step' method!").format(node->get_name());
			Godot::print_error(errorMsg,
				"EnvironmentNode::register_scene", __FILE__, __LINE__);
			quit(-1);
			return;
		}
		if (!node->has_method("reset")) {
			godot::String errorMsg = godot::String("Node '{0}' does not have the required 'reset' method!").format(node->get_name());
			Godot::print_error(errorMsg,
				"EnvironmentNode::register_scene", __FILE__, __LINE__);
			quit(-1);
			return;
		}

		mscene = node;
		mdone = false;
		mreward = 0.0;
	}

	void EnvironmentNode::_init() {
		// Keep this node running even when paused?
		//set_pause_mode(PAUSE_MODE_PROCESS);
	}

	void EnvironmentNode::setup() {

		// Check for command line args
		godot::PoolStringArray args = godot::OS::get_singleton()->get_cmdline_args();

		// Default port
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
			Godot::print(godot::String("Attempting to connect on port {0}").format(godot::Array::make(port)));
		}
		else {
			Godot::print("Failed to find a port argument, defaulting the port to 2048.");
		}

		// Attempt to connect
		if (!connect_agent(port)) {
			quit(-1);
			return;
		}
	}

	// Node added to scene tree
	void EnvironmentNode::_ready() {
		
	}

	void EnvironmentNode::quit(int code) {
		SceneTree* tree = get_tree();
		tree->quit(code);
	}

	void EnvironmentNode::_process(double delta) {
		if (!mhuman_control && !muse_physics_process) {
			run_step(delta);
		}
	}
	
	void EnvironmentNode::_physics_process(double delta) {
		if (!mhuman_control && muse_physics_process) {
			run_step(delta);
		}
	}

	void EnvironmentNode::run_step(double delta) {
		gdev::RequestType req = gdev::RequestType::None;

		mbuffer.clear();
		if (mcontext.recv(mbuffer)) {
			if (mbuffer.empty()) {
				ERR_PRINT("Received an empty message from the agent!");
				quit(-1);
				return;
			}

			const char* data = mbuffer.data();
			const char* end = mbuffer.data() + mbuffer.size();
			data = ez::deserialize::enumerator(data, end, req);

			switch (req) {
			default: {
				godot::String errorMsg = "Received an invalid value of '{0}' for the request type!";
				Godot::print_error(errorMsg.format(godot::Array::make(static_cast<int>(req))), "EnvironmentNode::_physics_process", __FILE__, __LINE__);
				quit(-1);
				break;
			}
			case gdev::RequestType::Close: {
				Godot::print("Closing the environment");
				quit(0);
				break;
			}
			case gdev::RequestType::Reset: {

				bool has_dict = false;
				data = ez::deserialize::value(data, end, has_dict);
				if (has_dict) {
					godot::Dictionary dict{};
					// Fill the dictionary
					gdev::deserialize_space(data, end, dict);

					mscene->call("reset", godot::Array::make(this, dict));
				}
				else {
					mscene->call("reset", godot::Array::make(this, godot::Dictionary{}));
				}

				mbuffer.clear();
				gdev::serialize_step(mobservation_space, mreward, mdone, mbuffer);
				if (!mcontext.send(mbuffer)) {
					Godot::print_error("Failed to send the step data!", "EnvironmentNode::_physics_process", __FILE__, __LINE__);
					quit(-1);
					return;
				}
				break;
			}
			case gdev::RequestType::Step: {
				// Step contains the action data

				// Deserialize the action data
				data = gdev::deserialize_space(data, end, maction_space);
				mscene->call("step", godot::Array::make(this));


				mbuffer.clear();
				// Prepare the step data
				gdev::serialize_step(mobservation_space, mreward, mdone, mbuffer);

				// Send the step data
				if (!mcontext.send(mbuffer)) {
					Godot::print_error("Failed to send the step data!", "EnvironmentNode::_physics_process", __FILE__, __LINE__);
					quit(-1);
					return;
				}
				break;
			}
			}
		}
		else {
			Godot::print_error("Failed to recv a message from the agent!", "EnvironmentNode::_physics_process", __FILE__, __LINE__);
			quit(-1);
			return;
		}
	}
}