#include <iostream>
#include <chrono>

#include <gdev/Agent.hpp>
#include "config.hpp"

namespace fs = std::filesystem;
namespace chrono = std::chrono;

using steady_clock = std::chrono::steady_clock;

int main() {
	fs::path godot = "godot";
	fs::path sceneFile = pendulum_environment_scene;
	fs::path projectDir = pendulum_project_dir;

	gdev::Agent agent;
	if (agent.hasEnvironment()) {
		std::cerr << "Agent is reporting it has an environment prior to createEnvironment call!\n";
		return -1;
	}
	if (agent.numInstances() != 0) {
		std::cerr << "Agent is reporting the incorrect number of instances!\n";
		return -1;
	}

	if (!agent.createEnvironment(godot, projectDir, sceneFile, 1)) {
		std::cerr << "Failed to create the environment!\n";
		std::cerr << "The executable was '" << godot << "'\n";
		std::cerr << "The scene file was '" << sceneFile << "'\n";
		return -1;
	}

	if (!agent.hasEnvironment()) {
		std::cerr << "Agent is incorrectly reporting that it does not have an environment after createEnvironment returned true!\n";
		return -1;
	}
	if (agent.numInstances() != 1) {
		std::cerr << "Agent is reporting the incorrect number of instances!\n";
		return -1;
	}
	
	auto time = steady_clock::now();

	// Run the main loop for the control
	gdev::Step step = agent.reset(0);
	

	return 0;
}