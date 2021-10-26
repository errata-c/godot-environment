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

	bool exec(const std::filesystem::path& godotExe, const std::filesystem::path& workingDir, const std::vector<std::string> & args) {
		namespace fs = std::filesystem;

		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		std::string command = godotExe.string();
		std::string working = workingDir.string();
		{
			std::size_t count = 0;
			for (const std::string & arg : args) {
				count += arg.size();
			}
			count += args.size();
			command.reserve(command.size() + count);
		}
		for (const std::string & arg : args) {
			command.append(1, ' ');
			command.append(arg);
		}

		BOOL result = CreateProcessA(
			NULL,
			command.data(),
			NULL,
			NULL,
			true,
			0,
			NULL,
			working.c_str(),
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