#pragma once
#include <Godot.hpp>
#include <Node.hpp>
#include <String.hpp>

#include <gdev/SpaceDef.hpp>
#include <gdev/Space.hpp>

#include <gdev/MessageContext.hpp>

namespace godot {
	/*
	Provide methods to define the Action space and the Observation space.
	Provide methods to set observation values.
	Get the command line arguments
	Integrate message context, setup the request reply loop.

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

		void set_action_space(godot::Dictionary space);
		void set_observation_space(godot::Dictionary space);

		void set_reward(double val);
		double get_reward() const noexcept;

		godot::Variant get_action(godot::String name, int index);
		void set_observation(godot::String name, int index, godot::Variant value);

		void print_action_space();
		void print_observation_space();

		// This function is automatically registered! It must exist for every class you make with godot.
		void _init();

		void _ready();

		void _process(float delta);
	private:
		gdev::MessageContext mcontext;

		gdev::SpaceDef acSpace, obSpace;
		gdev::Space observation, action;

		double reward;
	};
}