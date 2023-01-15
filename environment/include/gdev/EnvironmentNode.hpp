#pragma once
#include <Godot.hpp>
#include <Node.hpp>
#include <String.hpp>
#include <PackedScene.hpp>

#include <gdev/Space.hpp>

#include <gdev/MessageContext.hpp>

namespace godot {
	/*
	The main node to facilitate communication between an agent running in a different process and the actual environment in godot.
	This node must be instanced just once in a scene.
	*/
	class EnvironmentNode: public Node {
		GODOT_CLASS(EnvironmentNode, Node)
	public:
		static void _register_methods();

		EnvironmentNode();
		~EnvironmentNode();

		// Define the action space via (name, value) pairs in a dictionary
		void define_action_space(godot::Dictionary space);

		// Define the observation space via (name, value) pairs in a dictionary
		void define_observation_space(godot::Dictionary space);

		// Register the scene containing the environment
		void register_scene(godot::Node * scene);

		void set_connect_timeout(int ms);
		void set_send_timeout(int ms);
		void set_recv_timeout(int ms);
		bool connect_agent(int port);

		void setup();

		// Get the value of the done flag, used to signal that the environment has hit some kind of end condition.
		bool get_done() const noexcept;
		// Set the value of the done flag, used to signal that the environment has hit some kind of end condition.
		void set_done(bool val) noexcept;

		// Set the reward for the current step of the training.
		void set_reward(double val);
		// Get the previously set reward value for the current step of the training cycle.
		double get_reward() const noexcept;

		// This function is automatically registered! It must exist for every class you make with godot.
		void _init();

		// Called once the node has been created, and its children have been initialized.
		void _ready();
		
		void _physics_process(double delta);
		void _process(double delta);
		void run_step(double delta);

		// Attempt to quit the application instance
		void quit(int code);


		gdev::MessageContext mcontext;
		std::string mbuffer;

		godot::Node * mscene;
		
		// Simple dictionaries for the spaces, will require special serialization and deserialization.
		// Will make godot interfacing easier.
		godot::Dictionary maction_space, mobservation_space;

		bool mdone, muse_physics_process, mhuman_control;
		double mreward;
	};
}