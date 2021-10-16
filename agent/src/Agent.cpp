#include <gdev/Agent.hpp>

#include <cassert>

namespace gdev {
	const SpaceDef& Agent::actionSpace() const noexcept {
		return acSpace;
	}
	const SpaceDef& Agent::observationSpace() const noexcept {
		return obSpace;
	}

	int Agent::numEnvironments() const noexcept {
		return static_cast<int>(observations.size());
	}

	const Space& Agent::observation(int index) const {
		assert(index >= 0 && index < observations.size());

		return observations[index];
	}

	/*
	If the instances are instances of the game engine itself, than the vector of observations should also keep
	open connections in zmq.

	Otherwise we just need to send the index data.

	It seems like creating multiple instances of the environment per instance of the game engine is fairly easy.
	It may even be possible to have the EnvironmentNode class coordinate this behavior so that the work on the 
	Godot side of things is fairly minimal.

	Thus, for right now, that seems like the best bet.
	Provide the option for running headless, and for running split screen.
	Try to detect if the scene is 2d or 3d.
	*/
}