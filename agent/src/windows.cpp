#include <gdev/exec.hpp>

#include <windows.h>
#include <string>
#include <array>
#include <stdexcept>

namespace gdev {
	/*
	struct PrivateInstance {
		PROCESS_INFORMATION pi;
	};

	bool EnvInstance::isRunning() const {
		if (!isValid()) {
			return false;
		}

		DWORD exitCode;
		if (GetExitCodeProcess(pinst->pi.hProcess, &exitCode)) {
			return exitCode != STILL_ACTIVE;
		}
		else {
			return false;
		}
	}

	EnvInstance::~EnvInstance() {
		if (isValid()) {
			CloseHandle(pinst->pi.hProcess);
			CloseHandle(pinst->pi.hThread);

			free(pinst);
			pinst = nullptr;
		}
	}
	*/

	bool exec(const std::filesystem::path& godotExe, const std::filesystem::path& projectDir, int port) {
		namespace fs = std::filesystem;

		fs::file_status fstat = fs::status(projectDir);
		if (!fs::is_directory(fstat)) {
			throw std::logic_error("Project directory passed into gdev::exec was not a valid directory!");
		}

		if (port < 1024 || port > 65535) {
			throw std::logic_error("Port number passed into gdev::exec is not in the valid range of 1024 - 65535!");
		}

		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		std::string command = godotExe.string() + " --path " + projectDir.string() + " --port=" + std::to_string(port);

		BOOL result = CreateProcessA(
			command.c_str(),
			NULL,
			NULL,
			NULL,
			true,
			0,
			NULL,
			NULL,
			&si,
			&pi
		);

		if (result) {
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		return result;
	}
}