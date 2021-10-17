#pragma once

namespace gdev {
	enum class RequestType {
		None,
		Initialize,
		Reset,
		Step,
		Close,
	};
}