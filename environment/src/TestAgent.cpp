#include <gdev/TestAgent.hpp>

#include <gdev/RequestType.hpp>
#include <gdev/Serialize.hpp>
#include <ez/serialize/core.hpp>

#include <gdev/GodotConversions.hpp>

namespace godot {
	void TestAgent::_register_methods() {
		register_method("connect_environment", &TestAgent::connect_environment);
		register_method("reset", &TestAgent::reset);
		register_method("step", &TestAgent::step);
	}

	void TestAgent::_init() {
		
	}

	TestAgent::TestAgent()
		: mcontext(gdev::Com::Agent)
	{
		mcontext.setConnectTimeout(10 * 60 * 1000);
		mcontext.setRecvTimeout(10 * 60 * 1000);
		mcontext.setSendTimeout(10 * 60 * 1000);
	}

	bool TestAgent::connect_environment(int port) {
		if (!mcontext.connect(port)) {
			ERR_PRINT("TestAgent failed to connect to the environment!");
			return false;
		}

		return true;
	}
	Dictionary TestAgent::reset(Variant init_dict) {
		mbuffer.clear();
		ez::serialize::enumerator(gdev::RequestType::Reset, mbuffer);

		if (init_dict.get_type() != Variant::DICTIONARY) {
			ez::serialize::value(false, mbuffer);
		}
		else {
			ez::serialize::value(true, mbuffer);
			gdev::serialize_space(init_dict, mbuffer);
		}

		if (!mcontext.send(mbuffer)) {
			ERR_PRINT("Failed to send the reset command to the environment!");
			return {};
		}

		mbuffer.clear();
		if (!mcontext.recv(mbuffer)) {
			ERR_PRINT("Failed to receive response to reset command!");
			return {};
		}

		bool done = false;
		double reward = 0.0;
		Dictionary observation;

		gdev::deserialize_step(mbuffer.data(), mbuffer.data() + mbuffer.size(), observation, reward, done);
		Dictionary result;
		result["done"] = done;
		result["reward"] = reward;
		result["observation"] = observation;
		return result;
	}
	Dictionary TestAgent::step(Dictionary action) {
		mbuffer.clear();
		ez::serialize::enumerator(gdev::RequestType::Step, mbuffer);

		gdev::serialize_space(action, mbuffer);
		if (!mcontext.send(mbuffer)) {
			ERR_PRINT("Failed to send reset command!");
			return {};
		}

		mbuffer.clear();
		if (!mcontext.recv(mbuffer)) {
			ERR_PRINT("Failed to receive response to step command!");
			return {};
		}

		bool done = false;
		double reward = 0.0;
		Dictionary observation;

		gdev::deserialize_step(mbuffer.data(), mbuffer.data() + mbuffer.size(), observation, reward, done);
		Dictionary result;
		result["done"] = done;
		result["reward"] = reward;
		result["observation"] = observation;
		return result;
	}
}