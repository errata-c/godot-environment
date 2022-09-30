#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace gdev {
	static constexpr int portMin() noexcept {
		return 1024;
	}
	static constexpr int portMax() noexcept {
		return 65535;
	}

	void addPortArgument(int port, std::vector<std::string>& args);
	void addSceneArgument(const std::filesystem::path& scene, std::vector<std::string>& args);
	void addFixedFPSArgument(int fps, std::vector<std::string>& args);
	void addHeadlessArgument(std::vector<std::string>& args);
	void addEnvSeedArgument(int seed, std::vector<std::string>& args);
	void addSendTimeoutArgument(int ms, std::vector<std::string>& args);
	void addRecvTimeoutArgument(int ms, std::vector<std::string>& args);
	void addConnectTimeoutArgument(int ms, std::vector<std::string>& args);

	// Extra args are passed like --key=value to the actual executable.

	// system specific function to run the godot executable.
	// You must pass the path to the godot executable, the directory of the project, and the port to use for com.
	// The exact implementation for this function depends on the operating system this project will be compiled for.
	bool exec(
		const std::filesystem::path& godot,
		const std::filesystem::path& workingDir,
		const std::vector<std::string> & args);
}