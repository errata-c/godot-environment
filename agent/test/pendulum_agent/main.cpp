#include <iostream>
#include <chrono>
#include <random>

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

	gdev::Space action = agent.actionSpace().instance();
	action["force"].asReal() = 0.f;


	std::mt19937 gen(std::random_device{}());
	std::uniform_real_distribution<> dist(-1.f, 1.f);
	
	// Using std::endl to flush the text manually, otherwise it doesn't always print until after the loop
	std::cout << "\nBeginning the RL training loop" << std::endl;

	// Run the main loop for the control
	gdev::Step step = agent.reset(0);
	std::cout << "Received the first step data from the environment" << std::endl;

	auto startTime = steady_clock::now();

	std::size_t cycles = 0;
	while ((steady_clock::now() - startTime) < chrono::seconds(10)) {
		action["force"].asReal() = dist(gen)* 0.4f;
		step = agent.step(0, action);
		++cycles;
	}
	agent.close();

	std::cout << "Ended the RL training loop after " << cycles << " cycles." << std::endl;

	return 0;
}