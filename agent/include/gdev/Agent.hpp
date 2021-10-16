#pragma once
#include <vector>

#include <gdev/SpaceDef.hpp>
#include <gdev/ValueDef.hpp>
#include <gdev/Space.hpp>
#include <gdev/Value.hpp>

#include <gdev/MessageContext.hpp>

namespace gdev {
	struct Step {
		const Space& observation;
		double reward;
		bool done;
	};

	/*
	This class is the interface between the RL Agent you create in your own code base, and the godot environment.
	It will attempt to connect to a running instance of an environment, and allow the agent to interact with it.
	Internally it will store a list of observation spaces for each instance of the environment.
	
	I need to determine whether its going to run multiple instances of the game engine itself, or multiple instances of
	the environment inside a single instance of the game engine.

	Multiple instances of the game engine would make the c++ side more complex, but the godot side simpler.
	Multiple instances of the env in a single instance of the engine would be the reverse.
	*/
	class Agent {
	public:
		// The space of all actions
		const SpaceDef& actionSpace() const noexcept;

		// The space of all observations
		const SpaceDef& observationSpace() const noexcept;

		// Returns a reference to the most recent observation from an environment instance.
		const Space& observation(int index) const;

		// Take an action and advance the state of the environment.
		Step step(int index, const Space& action);

		// Completely finalize the environments, no further training will take place.
		// Note that this function does not accept an index, all active environments will be closed.
		void close();

		// Reset the environment for another (or first) training session, get the initial observation.
		Step reset(int index);

		// Returns the number of active environment instances.
		int numEnvironments() const noexcept;


	private:
		SpaceDef acSpace, obSpace;
		gdev::MessageContext mcontext;

		std::vector<Space> observations;		
	};
}