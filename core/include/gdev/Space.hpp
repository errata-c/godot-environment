#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include <string_view>
#include <optional>
#include <gdev/Value.hpp>

namespace gdev {
	/*
	A dictionary containing the possibilities for actions or observations.
	Each element in this dictionary is indexed by name, and returns a reference to a 4D tensor.
	The Tensor is of type Int, Bool, or Real
	*/
	class Space: public std::map<std::string, Value, std::less<std::string>> {
	public:
		// Use all the parent constructors
		using map::map;

		Space() = default;
		Space(const Space&) = default;
		Space(Space&&) noexcept = default;
		Space& operator=(const Space&) = default;
		Space& operator=(Space&&) noexcept = default;

		bool operator==(const Space& other) const noexcept;
		bool operator!=(const Space& otehr) const noexcept;
	};
}