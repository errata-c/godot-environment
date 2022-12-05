#include <iostream>
#include <string>
#include <vector>
#include <gdev/exec.hpp>
#include "config.hpp"

namespace fs = std::filesystem;

int main() {
	// Dead simple test to see if the exec function will work.
	fs::path godot = "godot";
	fs::path scene = pendulum_exec_scene;
	fs::path workingDir = pendulum_project_dir;

	std::vector<std::string> args;
	gdev::addSceneArgument(scene, args);

	if (!gdev::exec(godot, workingDir, args)) {
		std::cerr << "Failed to exec the scene file specified!\n";
		return -1;
	}

	return 0;
}