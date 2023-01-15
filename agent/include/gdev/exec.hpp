#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include <optional>

namespace gdev {
	static constexpr int port_min() noexcept {
		return 1024;
	}
	static constexpr int port_max() noexcept {
		return 65535;
	}

	struct ExecArgs {
		bool headless = false;
		int port = port_min();
		int send_timeout = 250;
		int recv_timeout = 250;
		int connect_timeout = 5000;
		std::optional<int> seed;
		std::optional<int> fixed_fps;
		std::optional<std::filesystem::path> project_path, scene_file, pack_file;
	};

	// system specific function to run the godot executable.
	// You must pass the path to the godot executable, the directory of the project, and the port to use for com.
	// The exact implementation for this function depends on the operating system this project will be compiled for.
	bool exec(
		const std::filesystem::path& godot,
		const std::filesystem::path& working_dir,
		const ExecArgs & args);
}