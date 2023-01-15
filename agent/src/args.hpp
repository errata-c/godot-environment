#pragma once
#include <gdev/exec.hpp>

namespace gdev {
	void addPortArgument(int port, std::vector<std::string>& args);
	void addProjectPathArgument(const std::filesystem::path& path, std::vector<std::string>& args);
	void addSceneArgument(const std::filesystem::path& scene, std::vector<std::string>& args);
	void addPackArgument(const std::filesystem::path& pack, std::vector<std::string>& args);
	void addFixedFPSArgument(int fps, std::vector<std::string>& args);
	void addHeadlessArgument(std::vector<std::string>& args);
	void addEnvSeedArgument(int seed, std::vector<std::string>& args);
	void addSendTimeoutArgument(int ms, std::vector<std::string>& args);
	void addRecvTimeoutArgument(int ms, std::vector<std::string>& args);
	void addConnectTimeoutArgument(int ms, std::vector<std::string>& args);

	std::vector<std::string> buildArgs(const ExecArgs& args);
}