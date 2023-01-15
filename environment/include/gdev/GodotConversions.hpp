#pragma once
#include <string_view>
#include <optional>

#include <Dictionary.hpp>
#include <Variant.hpp>

#include <gdev/Value.hpp>

namespace gdev {
	// This is where the conversion functions are defined, for transferring data between c++ and godot.

	// From Godot to c++

	// Name of the type as a string view (std library naming scheme)
	std::string_view to_string_view(godot::Variant::Type type);

	godot::String to_godot_string(godot::Variant::Type vtype);

	// Convert the variant to a ValueType enum, the var should be a string.
	std::optional<ValueType> to_value_type(godot::Variant var);
	
	// Convert the variant to a value range, used to describe the allowed values for an observation or action.
	std::optional<range_t> to_range(godot::Variant var);

	// Convert the variant to a set of dimensions for a tensor.
	std::optional<dims_t> to_dims(godot::Variant var);

	// From c++ to Godot

	godot::Variant to_variant(const gdev::ValueType& val);

	godot::Variant to_variant(const gdev::range_t& val);

	godot::Variant to_variant(const gdev::dims_t& val);

	template<typename T>
	std::optional<T> convert_to(const godot::Variant& var) {
		using godot::Variant;

		switch (var.get_type()) {
		case Variant::BOOL:
			if (bool(var)) {
				return static_cast<T>(1);
			}
			else {
				return static_cast<T>(0);
			}
		case Variant::INT: {
			int64_t ival(var);
			return static_cast<T>(ival);
		}
		case Variant::REAL: {
			double val(var);
			return static_cast<T>(val);
		}
		default:
			return std::nullopt;
		}
	}

	void serialize_space(const godot::Dictionary& value, std::string& buffer);
	void serialize_step(const godot::Dictionary& observation, double reward, bool done, std::string& buffer);

	const char* deserialize_space(const char* buffer, const char*const end, godot::Dictionary& value);
	const char* deserialize_step(const char* buffer, const char*const end, godot::Dictionary& observation, double& reward, bool& done);
}