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
		, minit(false)
	{}

	void Agent::setSendTimeout(int ms) {
		mcontext.setSendTimeout(std::max(ms, -1));
	}
	void Agent::setRecvTimeout(int ms) {
		mcontext.setRecvTimeout(std::max(ms, -1));
	}
	void Agent::setConnectTimeout(int ms) {
		mcontext.setConnectTimeout(std::max(ms, 0));
	}

	bool Agent::open(const std::filesystem::path& godot, const std::filesystem::path& projectDir, const std::filesystem::path& sceneFile) {
		if (isOpen()) {
			return false;
		}

		int port;
		{
			// Is this really the best solution?
			// We should have a more robust method for finding a valid port number, that checks to see if there is
			// a conflict somewhere.
			std::random_device rd;
			std::uniform_int_distribution<> dist(portMin(), portMax());
			std::mt19937 gen(rd());

			port = dist(gen);
		}

		std::vector<std::string> args;
		addSceneArgument(sceneFile, args);
		addPortArgument(port, args);
		addConnectTimeoutArgument(mcontext.getConnectTimeout(), args);
		addSendTimeoutArgument(mcontext.getSendTimeout(), args);
		addRecvTimeoutArgument(mcontext.getRecvTimeout(), args);

		// Attempt to run the exectuable
		bool result = exec(godot, projectDir, args);
		if (!result) {
			return false;
		}

		// The initialize the message context
		result = mcontext.connect(port);
		if (!result) {
			return false;
		}
		mobservation.clear();

		return true;
	}

	bool Agent::isOpen() const noexcept {
		return minit;
	}

	const Space& Agent::observation() const {
		return mobservation;
	}

	void Agent::close() {
		if (isOpen()) {
			mbuffer.clear();
			ez::serialize::enumerator(RequestType::Close, mbuffer);
			if (!mcontext.send(mbuffer)) {
				throw std::logic_error("Failed to send close command in gdev::Agent::close!");
			}
		}
		minit = false;
	}

	Step Agent::reset() {
		mbuffer.clear();
		ez::serialize::enumerator(RequestType::Reset, mbuffer);

		{
			int8_t has_dict = false;
			ez::serialize::i8(has_dict, mbuffer);
		}

		if (!mcontext.send(mbuffer)) {
			throw std::logic_error("Failed to send reset command in gdev::Agent::reset!");
		}

		mbuffer.clear();
		if (!mcontext.recv(mbuffer)) {
			throw std::logic_error("Failed to receive response to reset command in gdev::Agent::reset!");
		}

		bool done = false;
		float reward = 0.f;
		deserializeStep(mbuffer.data(), mbuffer.data() + mbuffer.size(), mobservation, reward, done);
		return Step{ mobservation, reward, done};
	}

	Step Agent::reset(const Space& init_dict) {
		mbuffer.clear();
		ez::serialize::enumerator(RequestType::Reset, mbuffer);

		{
			int8_t has_dict = true;
			ez::serialize::i8(has_dict, mbuffer);

			serialize(init_dict, mbuffer);
		}

		if (!mcontext.send(mbuffer)) {
			throw std::logic_error("Failed to send reset command in gdev::Agent::reset!");
		}

		mbuffer.clear();
		if (!mcontext.recv(mbuffer)) {
			throw std::logic_error("Failed to receive response to reset command in gdev::Agent::reset!");
		}

		bool done = false;
		float reward = 0.f;
		deserializeStep(mbuffer.data(), mbuffer.data() + mbuffer.size(), mobservation, reward, done);
		return Step{ mobservation, reward, done };
	}

	Step Agent::step(const Space& action) {
		mbuffer.clear();
		ez::serialize::enumerator(RequestType::Step, mbuffer);
		
		serialize(action, mbuffer);
		if (!mcontext.send(mbuffer)) {
			throw std::logic_error("Failed to send reset command in gdev::Agent::step!");
		}

		mbuffer.clear();
		if (!mcontext.recv(mbuffer)) {
			throw std::logic_error("Failed to receive response to reset command in gdev::Agent::step!");
		}

		bool done = false;
		float reward = 0.f;
		deserializeStep(mbuffer.data(), mbuffer.data() + mbuffer.size(), mobservation, reward, done);
		return Step{ mobservation, reward, done };
	}
}