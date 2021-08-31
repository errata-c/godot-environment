#include <gdev/MessageContext.hpp>
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {

	gdev::MessageContext ctx(gdev::Com::Agent);
	bool result = ctx.initialize(2048);
	if (result) {
		std::cout << "Successfully initialized a message context!\n";
	}
	else {
		std::cout << "Failed to initialize a message context!\n";
	}

	if (ctx.sendPing()) {
		std::cout << "Sent the ping message!\n";

		if (ctx.checkPing()) {
			std::cout << "Received a reply to our ping!\n";
		}
	}

	return 0;
}