#include <gdev/Agent.hpp>
#include <gdev/exec.hpp>

#include <gdev/RequestType.hpp>


#include <cassert>
#include <random>

namespace gdev {
	Agent::Agent()
		: mcontext(gdev::Com::Agent)
	{}

	const SpaceDef& Agent::actionSpace() const noexcept {
		return acSpace;
	}
	const SpaceDef& Agent::observationSpace() const noexcept {
		return obSpace;
	}

	bool Agent::createEnvironment(const std::filesystem::path& godot, const std::filesystem::path& scene, int count) {
		assert(count >= 0);
		assert(numInstances() == 0);

		int port;
		{
			std::random_device rd;
			std::uniform_int_distribution<> dist(portMin(), portMax());
			std::mt19937 gen(rd());

			port = dist(gen);
		}

		std::vector<std::string> args;
		addSceneArgument(scene, args);
		addPortArgument(port, args);

		// Attempt to run the exectuable
		bool result = exec(godot, args);
		if (!result) {
			return false;
		}

		// The initialize the message context
		result = mcontext.initialize(port);
		if (!result) {
			return false;
		}

		// Send the initialize request to the environment


		return false;
	}

	bool Agent::hasEnvironment() const noexcept {
		return numInstances() != 0;
	}

	int Agent::numInstances() const noexcept {
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