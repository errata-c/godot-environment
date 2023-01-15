#include <gdev/exec.hpp>
#include "args.hpp"

#include <string>
#include <sstream>
#include <stdexcept>

namespace gdev {
	namespace fs = std::filesystem;

	void addPortArgument(int port, std::vector<std::string>& args) {
		if (port < port_min() || port > port_max()) {
			throw std::logic_error("Port number '" + std::to_string(port) + "' is not in the valid range of [1024, 65535]!");
		}

		args.push_back("--port=" + std::to_string(port));
	}
	void addProjectPathArgument(const std::filesystem::path& path, std::vector<std::string>& args) {
		fs::file_status path_status = fs::status(path);
		if (fs::is_regular_file(path_status)) {
			fs::path ext = path.extension();
			if (ext != ".project") {
				throw std::logic_error("Project path points to a file with invalid extension!");
			}

			args.push_back('\"' + path.string() + '\"');
		}
		else if (fs::is_directory(path_status)) {
			fs::path project_file = path / "project.godot";
			if (!fs::is_regular_file(project_file)) {
				throw std::logic_error("Project path directory does not contain a project.godot file!");
			}

			args.push_back("--path");
			args.push_back('\"' + path.string() + '\"');
		}
		else {
			throw std::logic_error("Invalid project path argument!");
		}
	}
	void addSceneArgument(const fs::path& scene, std::vector<std::string>& args) {
		if (!scene.has_extension()) {
			std::string errorMsg = "Scene file '" + scene.string() + "' does not have a file extension!";
			throw std::logic_error(errorMsg);
		}

		fs::path ext = scene.extension();
		if (ext != ".tscn") {
			std::string errorMsg = "Scene file '" + scene.string() + "' does not have the correct extension!";
			throw std::logic_error(errorMsg);
		}

		args.push_back('\"' + scene.string() + '\"');
	}
	void addPackArgument(const std::filesystem::path& pack, std::vector<std::string>& args) {
		if (!pack.has_extension()) {
			std::string errorMsg = "Pack file '" + pack.string() + "' does not have a file extension!";
			throw std::logic_error(errorMsg);
		}

		fs::path ext = pack.extension();
		if (ext != ".pck") {
			std::string errorMsg = "Pack file '" + pack.string() + "' does not have the correct extension!";
			throw std::logic_error(errorMsg);
		}

		args.push_back("--main-pack");
		args.push_back('\"' + pack.string() + '\"');
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
	void addSendTimeoutArgument(int ms, std::vector<std::string>& args) {
		args.push_back("--send_timeout=" + std::to_string(ms));
	}
	void addRecvTimeoutArgument(int ms, std::vector<std::string>& args) {
		args.push_back("--recv_timeout=" + std::to_string(ms));
	}
	void addConnectTimeoutArgument(int ms, std::vector<std::string>& args) {
		args.push_back("--connect_timeout=" + std::to_string(ms));
	}

	std::vector<std::string> buildArgs(const ExecArgs& _args) {
		std::vector<std::string> args;

		if (_args.project_path) {
			gdev::addProjectPathArgument(*_args.project_path, args);
		}
		if (_args.scene_file) {
			gdev::addSceneArgument(*_args.scene_file, args);
		}
		if (_args.pack_file) {
			gdev::addPackArgument(*_args.pack_file, args);
		}

		gdev::addPortArgument(_args.port, args);
		gdev::addRecvTimeoutArgument(_args.recv_timeout, args);
		gdev::addSendTimeoutArgument(_args.send_timeout, args);
		gdev::addConnectTimeoutArgument(_args.connect_timeout, args);

		if (_args.headless) {
			gdev::addHeadlessArgument(args);
		}
		if (_args.seed) {
			gdev::addEnvSeedArgument(*_args.seed, args);
		}
		if (_args.fixed_fps) {
			gdev::addFixedFPSArgument(*_args.fixed_fps, args);
		}
		

		return args;
	}
}