#include <gdev/MessageContext.hpp>
#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <errno.h>

namespace gdev {
	MessageContext::MessageContext(Com _type)
		: ctx()
		, socket(ctx, _type == Com::Agent ? zmq::socket_type::req : zmq::socket_type::rep)
		, type(_type)
		, mport(-1)
	{}
	MessageContext::~MessageContext()
	{}

	bool MessageContext::isValid() const noexcept {
		return bool(ctx);
	}
	bool MessageContext::isConnected() const noexcept {
		return port() != -1;
	}

	void MessageContext::disconnect() {
		if (isConnected()) {
			socket.disconnect(address);
			mport = -1;
		}
	}

	bool MessageContext::initialize(int _port) {
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

		if (type == Com::Agent) {
			socket.connect(address);
		}
		else {
			socket.bind(address);
		}

		return true;
	}

	bool MessageContext::send(const std::vector<uint8_t>& buffer) {
		if (!isConnected()) {
			return false;
		}

		auto data = zmq::buffer(buffer.data(), buffer.size());
		zmq::send_result_t result = socket.send(data);

		return result.has_value();
	}

	bool MessageContext::recv(std::vector<uint8_t>& buffer) {
		if (!isConnected()) {
			return false;
		}

		zmq::message_t message;
		zmq::recv_result_t result = socket.recv(message);

		if (!result.has_value()) {
			return false;
		}

		const uint8_t * data = (const uint8_t*)message.data();
		const uint8_t * last = data + message.size();

		for (; data != last; ++data) {
			buffer.push_back(*data);
		}

		return true;
	}

	bool MessageContext::sendPing() {
		if (!isConnected()) {
			return false;
		}

		try {
			auto result = socket.send(zmq::str_buffer("ping"), zmq::send_flags::dontwait);
			if (!result.has_value()) {
				return false;
			}
		}
		catch (zmq::error_t & err) {
			std::cout << err.what() << '\n';
			return false;
		}
		
		return true;
	}

	bool MessageContext::checkPing() {
		if (!isConnected()) {
			return false;
		}

		zmq::message_t message;
		auto result = socket.recv(message);

		if (result.has_value()) {
			std::string_view rec((const char *)message.data(), message.size());
			std::string_view compare("ping");
			return rec == compare;
		}
		else {
			return false;
		}
	}

	int MessageContext::port() const noexcept {
		return mport;
	}
}