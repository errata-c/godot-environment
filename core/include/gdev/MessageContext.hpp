#pragma once
#include <string>
#include <cinttypes>

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

		void setSendTimeout(int ms);
		void setRecvTimeout(int ms);
		void setConnectTimeout(int ms);

		int getSendTimeout() const;
		int getRecvTimeout() const;
		int getConnectTimeout() const;

		bool isValid() const noexcept;
		bool isConnected() const noexcept;
		
		bool connect(int _port);
		void disconnect();

		// Attempt to send a buffer of data to the other context.
		bool send(const std::string & buffer);

		// Attempt to receive a buffer of data from the other context.
		bool recv(std::string & buffer);

		int port() const noexcept;
	private:
		Com type;
		int mport;
		std::string address;

		void* ctx;
		void*  socket;
	};
}