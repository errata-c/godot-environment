#include <gdev/exec.hpp>

#include <string>
#include <sstream>
#include <stdexcept>

namespace gdev {
	void addPortArgument(int port, std::vector<std::string>& args) {
		if (port < portMin() || port > portMax()) {
			throw std::logic_error("Port number '" + std::to_string(port) + "' is not in the valid range of [1024, 65535]!");
		}

		args.push_back("--port=" + std::to_string(port));
	}
	void addSceneArgument(const std::filesystem::path& scene, std::vector<std::string>& args) {
		namespace fs = std::filesystem;

		if (!scene.has_extension()) {
			std::string errorMsg = "Scene file '" + scene.string() + "' does not have a file extension!";
			throw std::logic_error(errorMsg);
		}

		fs::path ext = scene.extension();
		if (ext != ".tscn") {
			std::string errorMsg = "Scene file '" + scene.string() + "' does not have the correct extension!";
			throw std::logic_error(errorMsg);
		}

		args.push_back(scene.string());
	}
	void addFixedFPSArgument(int fps, std::vector<std::string>& args) {
		if (fps < 1) {
			throw std::logic_error("FPS less than 1 requested!");
		}

		args.push_back("--fixed-fps");
		args.push_back(std::to_string(fps));
	}
	void addHeadlessArgument(std::vector<std::string>& args) {
		args.push_back("--disable-render-loop");
		args.push_back("--no-window");
	}
	void addEnvSeedArgument(int seed, std::vector<std::string>& args) {
		args.push_back("--env_seed=" + std::to_string(seed));
	}
}