#pragma once
#include <string>
#include <filesystem>
#include <cinttypes>

#include <gdev/Space.hpp>
#include <gdev/Value.hpp>
#include <gdev/Step.hpp>

#include <gdev/MessageContext.hpp>

namespace gdev {
	/*
	This class is the interface between the RL Agent you create in your own code base, and the godot environment.
	It will attempt to connect to a running instance of an environment, and allow the agent to interact with it.
	*/
	class Agent {
	public:
		Agent();
		Agent(Agent &&) noexcept = default;

		void setSendTimeout(int ms);
		void setRecvTimeout(int ms);
		void setConnectTimeout(int ms);
		
		// Attempt to run the godot executable (name passed as the 'godot' param)
		// using the scene specified
		bool open(
			const std::filesystem::path& godot, 
			const std::filesystem::path& projectDir, 
			const std::filesystem::path& sceneFile);

		// Completely finalize the environments, no further training will take place.
		void close();

		// Returns true if the environment has been created already.
		bool isOpen() const noexcept;

		// Returns a reference to the most recent observation from an environment instance.
		const Space& observation() const;

		// Take an action and advance the state of the environment.
		Step step(const Space& action);

		// Reset the environment for another (or first) training session, get the initial observation.
		Step reset();

		// Reset the environment for another (or first) training session, get the initial observation.
		// Pass in a dictionary of initialization parameters
		Step reset(const Space& init_dict);
	private:
		bool minit;
		gdev::MessageContext mcontext;

		std::string mbuffer;
		Space mobservation;
	};
}