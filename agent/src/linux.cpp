#include <gdev/exec.hpp>

#include <string>
#include <array>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>

static void childHandler(int sig) {}

namespace gdev {
	bool exec(const std::filesystem::path& godotExe, const std::filesystem::path& projectDir, int port) {
		namespace fs = std::filesystem;

		fs::file_status fstat = fs::status(projectDir);
		if (!fs::is_directory(fstat)) {
			throw std::logic_error("Project directory passed into gdev::exec was not a valid directory!");
		}

		if (port < 1024 || port > 65535) {
			throw std::logic_error("Port number passed into gdev::exec is not in the valid range of 1024 - 65535!");
		}

		std::string godot = godotExe.string();
		std::string proj = projectDir.string();
		std::string portArg = "--port=";
		portArg += std::to_string(port);

		std::array<char*, 4> args{
			"--path",
			proj.c_str(),
			portArg.c_str(),
			nullptr // MUST BE NULL TERMINATED, c functions are barbaric
		};

		// Setup the signal so we don't have to do anything special to terminate the child processes.
		struct sigaction sa;

		sa.sa_sigaction = childHandler;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART | SA_SIGINFO | SA_NOCLDSTOP;

		sigaction(SIGCHLD, &sa, NULL);


		// Branch onto a new thread.
		pid_t pid = fork();
		// pid will be zero in child process
		// else parent process.

		if (pid == 0) {
			// Execute the process
			if (execvp(godot.c_str(), args.data()) == -1) {
				// This only returns if an error occurs
				// At that point, just terminate the forked process.
				
				std::terminate();
			}
		}
		else {
			// Parent process should probably wait a bit to see if the child process is actually running.
			return true;
		}
	}
}