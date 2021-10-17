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
	bool exec(const std::filesystem::path& godotExe, const std::vector<std::string> & args) {
		namespace fs = std::filesystem;

		std::string godot = godotExe.string();

		// +1, because it needs to be null terminated
		std::vector<const char *> cargs(args.size()+1, nullptr);
		for (std::size_t i = 0; i < args.size(); ++i) {
			cargs[i] = args[i].c_str();
		}

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
			if (execvp(godot.c_str(), cargs.data()) == -1) {
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