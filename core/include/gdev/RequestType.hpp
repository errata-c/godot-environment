#pragma once

namespace gdev {
	enum class RequestType {
		// Empty request, will get an empty response
		// Good for a simple 'ping' test
		None,

		// Reset the environment
		Reset,

		// Step the environment
		Step,

		// Close the connection to the environment
		Close,

		SetRecvTimeout,
		SetSendTimeout,
	};
}