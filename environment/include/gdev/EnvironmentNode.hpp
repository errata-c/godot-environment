#pragma once
#include <Godot.hpp>
#include <Node.hpp>
#include <String.hpp>
#include <PackedScene.hpp>

#include <gdev/SpaceDef.hpp>
#include <gdev/Space.hpp>

#include <gdev/MessageContext.hpp>

namespace godot {
	/*

	*/
	class EnvironmentNode: public Node {
		GODOT_CLASS(EnvironmentNode, Node)
	public:
		static void _register_methods();

		EnvironmentNode();
		~EnvironmentNode();
		
		gdev::SpaceDef & actionSpaceDef() noexcept;
		const gdev::SpaceDef& actionSpaceDef() const noexcept;

		gdev::SpaceDef & observationSpaceDef() noexcept;
		const gdev::SpaceDef& observationSpaceDef() const noexcept;

		// Define the action space via (name, value) pairs in a dictionary
		void define_action_space(godot::Dictionary space);
		// Define the observation space via (name, value) pairs in a dictionary
		void define_observation_space(godot::Dictionary space);

		// Register the scene containing the environment, this is necessary to allow for instancing.
		void register_scene(godot::String scene);

		// Get the value of the done flag, used to signal that the environment has hit some kind of end condition.
		bool get_done() const noexcept;
		// Set the value of the done flag, used to signal that the environment has hit some kind of end condition.
		void set_done(bool val) noexcept;

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

		// Called once the node has been created, and its children have been initialized.
		void _ready();

		// Called at a (supposedly) fixed rate
		void _physics_process(float delta);

		// Attempt to quit the application instance
		void quit(int code);

		struct Instance {
			Instance();
			Instance(godot::Node * n);

			godot::Node * node;
			gdev::Space observation, action;

			bool done;
			double reward;
		};
	private:
		gdev::MessageContext mcontext;
		std::string buffer;
		
		gdev::SpaceDef acSpace, obSpace;

		int32_t currentIndex;
		std::vector<Instance> instances;

		void prepareInstances(int numInstances, const Ref<PackedScene> & scene);
	};
}