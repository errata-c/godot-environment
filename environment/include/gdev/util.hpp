#pragma once
#include <string_view>
#include <optional>
#include <gdev/ValueDef.hpp>
#include <Variant.hpp>

namespace gdev {
	// From Godot to c++
	std::string_view to_string_view(godot::Variant::Type type);
	std::optional<ValueDef> convertToValueDef(godot::Variant var);
	std::optional<ValueType> convertToValueType(godot::Variant var);
	std::optional<range_t> convertToRange(godot::Variant var);
	std::optional<dim_t> convertToDims(godot::Variant var);

	// From c++ to Godot
	godot::Variant convertToVariant(const gdev::Value & val);
}