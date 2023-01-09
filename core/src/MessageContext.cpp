#include <gdev/MessageContext.hpp>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <cstdio>
#include <errno.h>

#include <zmq.h>

namespace gdev {
	MessageContext::MessageContext(MessageContext&& other) noexcept {
		ctx = other.ctx;
		socket = other.socket;
		type = other.type;
		mport = other.mport;
	}
	MessageContext& MessageContext::operator=(MessageContext&& other) noexcept {
		this->~MessageContext();
		new (this) MessageContext(std::move(other));

		return *this;
	}

	MessageContext::MessageContext(Com _type)
		: ctx(nullptr)
		, socket(nullptr)
//		, socket(ctx, _type == Com::Agent ? zmq::socket_type::req : zmq::socket_type::rep)
		, type(_type)
		, mport(-1)
	{
		ctx = zmq_ctx_new();
		if (!ctx) {
			// Throw exception, couldn't even get a context!
			assert(false);
		}

		socket = zmq_socket(ctx, type == Com::Agent ? ZMQ_REQ : ZMQ_REP);
		if(!socket) {
			// Couldn't create a socket
			// Cleanup the context?
			assert(false);
		}

		int error = 0;
		{
			// 30 seconds by default
			// The connect timeout should be longer as the environment might take some time to open.
			int value = 30000;
			error = zmq_setsockopt(socket, ZMQ_CONNECT_TIMEOUT, &value, sizeof(value));

			// Should succeed
			assert(error == 0);
		}
		{
			// 4 seconds by default
			int value = 4000;
			error = zmq_setsockopt(socket, ZMQ_RCVTIMEO, &value, sizeof(value));

			// Should succeed
			assert(error == 0);
		}
		{
			// 4 seconds by default
			int value = 4000;
			error = zmq_setsockopt(socket, ZMQ_SNDTIMEO, &value, sizeof(value));

			// Should succeed
			assert(error == 0);
		}
	}
	MessageContext::~MessageContext()
	{
		int error = 0;
		if (socket) {
			error = zmq_close(socket);
			assert(error == 0); // Should succeed

			socket = nullptr;
		}
		if (ctx) {
			error = zmq_ctx_term(ctx);
			assert(error == 0); // Might return EINTR, but I don't know what should be done in that case...

			ctx = nullptr;
		}
	}

	void MessageContext::setSendTimeout(int ms) {
		assert(socket);
		// Only changes the timeout for subsequent connections, not current ones.

		int error = zmq_setsockopt(socket, ZMQ_SNDTIMEO, &ms, sizeof(ms));
		assert(error == 0);
	}
	void MessageContext::setRecvTimeout(int ms) {
		assert(socket);
		// Only changes the timeout for subsequent connections, not current ones.

		int error = zmq_setsockopt(socket, ZMQ_RCVTIMEO, &ms, sizeof(ms));
		assert(error == 0);
	}
	void MessageContext::setConnectTimeout(int ms) {
		assert(socket);
		// Only changes the timeout for subsequent connections, not current ones.

		int error = zmq_setsockopt(socket, ZMQ_CONNECT_TIMEOUT, &ms, sizeof(ms));
		assert(error == 0);
	}

	int MessageContext::getSendTimeout() const {
		int ms = 0;
		size_t len = 0;
		int error = zmq_getsockopt(socket, ZMQ_SNDTIMEO, &ms, &len);
		assert(error == 0);

		return ms;
	}
	int MessageContext::getRecvTimeout() const {
		int ms = 0;
		size_t len = 0;
		int error = zmq_getsockopt(socket, ZMQ_RCVTIMEO, &ms, &len);
		assert(error == 0);

		return ms;
	}
	int MessageContext::getConnectTimeout() const {
		int ms = 0;
		size_t len = 0;
		int error = zmq_getsockopt(socket, ZMQ_CONNECT_TIMEOUT, &ms, &len);
		assert(error == 0);

		return ms;
	}

	bool MessageContext::isValid() const noexcept {
		return bool(ctx);
	}
	bool MessageContext::isConnected() const noexcept {
		return port() != -1;
	}

	void MessageContext::disconnect() {
		if (isConnected()) {
			int error = 0;
			if (type == Com::Agent) {
				error = zmq_disconnect(socket, address.data());
			}
			else {
				error = zmq_unbind(socket, address.data());
			}
			assert(error == 0);

			mport = -1;
		}
	}

	bool MessageContext::connect(int _port) {
		// Dont initialize if we are already in some state.
		if (isConnected()) {
			return false;
		}
		if (_port < 1024 || _port > 65535) {
			return false;
		}
		mport = _port;

		// Create the TCP address, localhost loopback, custom port for id
		address.resize(32, '\0');
		int count = snprintf(address.data(), address.size(), "tcp://127.0.0.1:%d", port());
		assert(count > 0);

		address.resize(count);

		int error = 0;

		if (type == Com::Agent) {
			error = zmq_connect(socket, address.data());
		}
		else {
			error = zmq_bind(socket, address.data());
		}

		switch (error) {
		case 0:
			// No error
			return true;

		case EINVAL:
			break;
		case EPROTONOSUPPORT:
			break;
		case ENOCOMPATPROTO:
			break;
		
		case EADDRINUSE:
			break;
		case EADDRNOTAVAIL:
			break;

		case ETERM:
			break;
		case ENODEV:
			break;
		case ENOTSOCK:
			break;
		case EMTHREAD:
			break;
		default:
			// Unexpected error?
			break;
		}

		mport = -1;
		return false;
	}

	bool MessageContext::send(const std::string& buffer) {
		if (!isConnected()) {
			return false;
		}

		int nbytes = zmq_send(socket, buffer.data(), buffer.size(), 0);
		if (nbytes == -1) {
			int error = errno;
			if (error == EFSM) {
				// Socket was not in the correct state, the request reply loop was not respected.
				assert(false); // Replace with an exception
			}
			return false;
		}
		assert(nbytes == buffer.size());
		
		return true;
	}

	bool MessageContext::recv(std::string& buffer) {
		if (!isConnected()) {
			return false;
		}

		zmq_msg_t message;
		int error = zmq_msg_init(&message);
		assert(error == 0);

		int nbytes = zmq_msg_recv(&message, socket, 0);
		assert(nbytes != -1);

		const char* data = (const char*)zmq_msg_data(&message);
		assert(data != nullptr);
		const char* last = data + nbytes;

		std::copy(data, last, std::back_inserter(buffer));

		zmq_msg_close(&message);

		return true;
	}

	int MessageContext::port() const noexcept {
		return mport;
	}
}