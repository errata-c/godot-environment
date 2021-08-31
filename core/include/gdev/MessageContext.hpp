#pragma once
#include <string>
#include <vector>
#include <cinttypes>
#include <zmq.hpp>

namespace gdev {
	// What end of the communications are we?
	enum class Com {
		Agent,
		Environment,
	};

	// This class will handle all the specifics surrounding zeromq messaging.
	// Allow for transmitting data between the Godot engine and the Agent executable.
	class MessageContext {
	public:
		MessageContext(const MessageContext &) = delete;
		MessageContext& operator=(const MessageContext&) = delete;

		MessageContext(MessageContext && other) noexcept;
		MessageContext& operator=(MessageContext &&) noexcept;

		MessageContext(Com _type);
		~MessageContext();

		bool isValid() const noexcept;
		bool isConnected() const noexcept;

		// Address must be between 1 and 255
		bool initialize(int _port);
		void disconnect();

		bool send(const std::vector<uint8_t> & buffer);
		bool recv(std::vector<uint8_t> & buffer);

		// Send a ping message to check if the server is ready.
		bool sendPing();

		// Check for a ping reply.
		bool checkPing();
	private:
		Com type;
		int port;
		std::string address;

		zmq::context_t ctx;
		zmq::socket_t socket;
	};
}