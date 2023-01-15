#include <iostream>
#include <string>
#include <vector>
#include <gdev/exec.hpp>
#include "config.hpp"

namespace fs = std::filesystem;

int main() {
	// Dead simple test to see if the exec function will work.
	fs::path godot = "godot";
	fs::path working_dir = pendulum_project_dir;

	gdev::ExecArgs args;
	args.scene_file = pendulum_exec_scene;
	args.project_path = pendulum_project_dir;

	if (!gdev::exec(godot, working_dir, args)) {
		std::cerr << "Failed to exec the scene file specified!\n";
		return -1;
	}

	return 0;
}