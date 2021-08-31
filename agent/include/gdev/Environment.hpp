#pragma once
#include <gdev/SpaceDef.hpp>
#include <gdev/ValueDef.hpp>
#include <gdev/Space.hpp>
#include <gdev/Value.hpp>



namespace gdev {
	struct Step {
		const Space& observation;
		double reward;
		bool done;
	};

	/*
		This class handles the interfacing with the Godot game engine.
		It will attempt to connect to a running instance of an environment, and allow the agent to interact with it.
	*/
	class Environment {
	public:
		// The space of all actions
		const SpaceDef& actionSpace() const;

		// The space of all observations
		const SpaceDef& observationSpace() const;
		const Space& observation() const;

		// Take an action and advance the state of the environment.
		Step step(const Space& action);

		// Completely finalize the environment, no further training will take place.
		void close();

		// Reset the environment for another (or first) training session, get the initial observation.
		Step reset();

	private:
		SpaceDef acSpace, obSpace;
		Space currentObservation;


	};
}