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

	/*
	Class that handles the connection between the Agent and the Environment.
	Provides only the bare minimum capabilities, only allows sending and receiving byte buffers.
	This puts the responsibility of interpreting the data on the users of this class.

	This class operates in a strict request-reply loop, with the Agent sending the requests,
	and the Environment sending the replies.
	*/
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
		
		bool initialize(int _port);
		void disconnect();

		// Attempt to send a buffer of data to the other context.
		bool send(const std::vector<uint8_t> & buffer);

		// Attempt to receive a buffer of data from the other context.
		bool recv(std::vector<uint8_t> & buffer);

		// Send a ping message to check if the server is ready.
		bool sendPing();

		// Check for a ping reply.
		bool checkPing();

		int port() const noexcept;
	private:
		Com type;
		int mport;
		std::string address;

		zmq::context_t ctx;
		zmq::socket_t socket;
	};
}