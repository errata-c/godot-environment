#pragma once
#include <string_view>
#include <optional>
#include <gdev/ValueDef.hpp>
#include <Variant.hpp>

namespace gdev {
	std::string_view to_string_view(godot::Variant::Type type);
	std::optional<ValueDef> convertToValueDef(godot::Variant var);
	std::optional<ValueType> convertToValueType(godot::Variant var);
	std::optional<double> convertToRange(godot::Variant var);
}