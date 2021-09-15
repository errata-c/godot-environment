#pragma once
#include <Godot.hpp>
#include <Node.hpp>
#include <String.hpp>

#include <gdev/SpaceDef.hpp>
#include <gdev/Space.hpp>

#include <gdev/MessageContext.hpp>

namespace godot {
	/*
	Node to describe and drive the runtime loop on the godot side of things.


	Provide methods to define the Action space and the Observation space.
	Provide methods to set observation values.
	Get the command line arguments
	Integrate message context, setup the request reply loop.

	*/
	class EnvironmentNode: public Node {
		GODOT_CLASS(EnvironmentNode, Node)
	public:
		enum class State {
			None, // No requests made yet.
			SendDefs, // The Agent has requested the action and observation space defs
			Reset, // The Agent has requested a reset of the environment
			Step, // The Agent has sent action data and requested stepping the environment
			Close, // The Agent has requested the environment to close down.
		};

		static void _register_methods();

		EnvironmentNode();
		~EnvironmentNode();
		
		gdev::SpaceDef & actionSpaceDef() noexcept;
		const gdev::SpaceDef& actionSpaceDef() const noexcept;

		gdev::SpaceDef & observationSpaceDef() noexcept;
		const gdev::SpaceDef& observationSpaceDef() const noexcept;

		bool reset_requested() const noexcept;
		bool step_requested() const noexcept;
		bool send_defs_requested() const noexcept;
		bool close_requested() const noexcept;

		// Get the value of the done flag, used to signal that the environment has hit some kind of end condition.
		bool get_done() const noexcept;
		// Set the value of the done flag, used to signal that the environment has hit some kind of end condition.
		bool set_done(bool val) noexcept;

		// Set the action space definition.
		void set_action_space(godot::Dictionary space);
		// Set the observation space definition.
		void set_observation_space(godot::Dictionary space);

		// Set the reward for the current step of the training.
		void set_reward(double val);
		// Get the previously set reward value for the current step of the training cycle.
		double get_reward() const noexcept;

		// Get an action value under the specified name.
		// Note that tensors are always returned as single dimension arrays, to prevent excessive allocations.
		godot::Variant get_action(godot::String name);

		// Find and set the observation under the provided name.
		// Note that to write a tensor you need to provide either a single dimension array with the number of elements
		// or a multidimension array of the correct dims.
		void set_observation(godot::String name, godot::Variant value);

		// Print the current definition of the action space, useful for debugging.
		void print_action_space_def();
		// Print the current definition of the observation space, useful for debugging.
		void print_observation_space_def();

		// This function is automatically registered! It must exist for every class you make with godot.
		void _init();

		void _ready();

		void _process(float delta);
	private:
		gdev::MessageContext mcontext;

		gdev::SpaceDef acSpace, obSpace;
		gdev::Space observation, action;

		State state;
		bool done;
		double reward;
	};
}