#include <gdev/Space.hpp>
#include <utility>
#include <stdexcept>
#include <fmt/format.h>

namespace gdev {
	using iterator = Space::iterator;
	using const_iterator = Space::const_iterator;

	bool Space::operator==(const Space& other) const noexcept {
		if (size() != other.size()) {
			return false;
		}
		for (const auto & elem : *this) {
			auto it = other.find(elem.first);
			if (it == other.end()) {
				return false;
			}
			else if(it->second != elem.second) {
				return false;
			}
		}
		return true;
	}
	bool Space::operator!=(const Space& other) const noexcept {
		if (size() != other.size()) {
			return true;
		}
		for (const auto& elem : *this) {
			auto it = other.find(elem.first);
			if (it == other.end()) {
				return true;
			}
			else if (it->second != elem.second) {
				return true;
			}
		}
		return false;
	}
}