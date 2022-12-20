#pragma once
#include <exception>
#include <stdexcept>

namespace gdev {
	class invalid_type: public std::logic_error {
	public:
		explicit invalid_type(const std::string& what_arg);
		explicit invalid_type(const char* what_arg);
	};
}