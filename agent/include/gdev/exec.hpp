#pragma once
#include <filesystem>

namespace gdev {
	/*
	struct PrivateInstance;

	// Instance of the environment
	class EnvInstance {
	public:
		EnvInstance() noexcept
			: pinst(nullptr)
		{}
		EnvInstance(PrivateInstance* ptr) noexcept
			: pinst(ptr)
		{}

		EnvInstance(EnvInstance && other) noexcept
			: pinst(other.pinst)
		{}
		

		explicit operator bool() const noexcept {
			return isValid();
		}
		bool isValid() const noexcept {
			return pinst != nullptr;
		}


		bool isRunning() const;

		EnvInstance& operator=(EnvInstance&&) noexcept;

		~EnvInstance();
	private:
		PrivateInstance * pinst;
	};
	*/

	// Extra args are passed like --key=value

	// system specific function to run the godot executable.
	// You must pass the path to the godot executable, the directory of the project, and the port to use for com.
	bool exec(const std::filesystem::path& godotExe, const std::filesystem::path& projectDir, int port);
}