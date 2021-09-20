#pragma once

namespace gdev {
	enum class RequestType {
		None,
		SendDefs,
		Reset,
		Step,
		Close,
	};
}