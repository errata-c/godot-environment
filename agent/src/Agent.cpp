#include <gdev/Agent.hpp>
#include <gdev/exec.hpp>

#include <gdev/RequestType.hpp>
#include <gdev/Serialize.hpp>
#include <ez/serialize.hpp>

#include <cassert>
#include <random>
#include <stdexcept>

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

	bool Agent::createEnvironment(const std::filesystem::path& godot, const std::filesystem::path& projectDir, const std::filesystem::path& sceneFile, int count) {
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
		addSceneArgument(sceneFile, args);
		addPortArgument(port, args);

		// Attempt to run the exectuable
		bool result = exec(godot, projectDir, args);
		if (!result) {
			return false;
		}

		// The initialize the message context
		result = mcontext.initialize(port);
		if (!result) {
			return false;
		}

		// Send the initialize request to the environment
		buffer.clear();
		int32_t instances = count;
		serialize(RequestType::Initialize, buffer);
		ez::serialize::i32(instances, buffer);

		if (!mcontext.send(buffer)) {
			return false;
		}

		buffer.clear();
		if (!mcontext.recv(buffer)) {
			return false;
		}
		deserializeDefs(buffer.data(), buffer.data() + buffer.size(), acSpace, obSpace);

		observations.clear();
		for (int i = 0; i < count; ++i) {
			observations.emplace_back();
		}

		return true;
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

	void Agent::close() {
		buffer.clear();
		serialize(RequestType::Close, buffer);
		if (!mcontext.send(buffer)) {
			throw std::logic_error("Failed to send close command in gdev::Agent::close!");
		}
	}

	Step Agent::reset(int index) {
		if (index < 0 || index > numInstances()) {
			throw std::out_of_range("Call to gdev::Agent::reset with an invalid index!");
		}

		buffer.clear();
		serialize(RequestType::Reset, buffer);

		Space & obs = observations[index];
		{
			int32_t i = index;
			ez::serialize::i32(i, buffer);
			if (!mcontext.send(buffer)) {
				throw std::logic_error("Failed to send reset command in gdev::Agent::reset!");
			}
		}

		buffer.clear();
		if (!mcontext.recv(buffer)) {
			throw std::logic_error("Failed to receive response to reset command in gdev::Agent::reset!");
		}

		bool done = false;
		float reward = 0.f;
		deserializeStep(buffer.data(), buffer.data() + buffer.size(), obs, reward, done);
		return Step{obs, reward, done};
	}

	Step Agent::step(int index, const Space& action) {
		if (index < 0 || index > numInstances()) {
			throw std::out_of_range("Call to gdev::Agent::step with an invalid index!");
		}

		buffer.clear();
		serialize(RequestType::Step, buffer);

		Space& obs = observations[index];

		{
			int32_t i = index;
			ez::serialize::i32(i, buffer);
			serialize(action, buffer);
			if (!mcontext.send(buffer)) {
				throw std::logic_error("Failed to send reset command in gdev::Agent::step!");
			}
		}

		buffer.clear();
		if (!mcontext.recv(buffer)) {
			throw std::logic_error("Failed to receive response to reset command in gdev::Agent::step!");
		}

		bool done = false;
		float reward = 0.f;
		deserializeStep(buffer.data(), buffer.data() + buffer.size(), obs, reward, done);
		return Step{ obs, reward, done };
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