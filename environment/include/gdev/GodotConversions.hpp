#pragma once
#include <string_view>
#include <optional>
#include <gdev/ValueDef.hpp>
#include <Variant.hpp>

namespace gdev {
	// This is where the conversion functions are defined, for transferring data between c++ and godot.

	// From Godot to c++

	// Name of the type as a string view (std library naming scheme)
	std::string_view to_string_view(godot::Variant::Type type);
	// Convert the variant to a ValueDef
	std::optional<ValueDef> convertToValueDef(godot::Variant var);
	// Convert the variant to a ValueType enum, the var should be a string.
	std::optional<ValueType> convertToValueType(godot::Variant var);
	// Convert the variant to a value range, used to describe the allowed values for an observation or action.
	std::optional<range_t> convertToRange(godot::Variant var);
	// Convert the variant to a set of dimensions for a tensor.
	std::optional<dim_t> convertToDims(godot::Variant var);
	// Convert the variant to a Value
	std::optional<Value> convertToValue(godot::Variant var);

	// From c++ to Godot

	// Take the Value and convert it to a godot::Variant.
	godot::Variant convertToVariant(const gdev::Value & val);

	godot::Variant convertToVariant(const gdev::ValueDef & val);
	godot::Variant convertToVariant(const gdev::ValueType& val);
	godot::Variant convertToVariant(const gdev::range_t& val);
	godot::Variant convertToVariant(const gdev::dim_t& val);
}