#include <gdev/util.hpp>

#include <Dictionary.hpp>
#include <String.hpp>
#include <Godot.hpp>

#include <iostream>

using VType = godot::Variant::Type;
using Godot = godot::Godot;

namespace gdev {
	std::optional<ValueType> convertToValueType(godot::Variant var) {
		if (var.get_type() != VType::STRING) {
			return std::nullopt;
		}
		godot::String tmp = var;
		godot::String typestring = tmp.to_lower();

		if (typestring == "real") {
			return gdev::ValueType::Real;
		}
		else if (typestring == "category" || typestring == "categorical") {
			return gdev::ValueType::Int;
		}
		else if (typestring == "binary") {
			return gdev::ValueType::Bool;
		}
		else {
			return std::nullopt;
		}
	}
	std::optional<double> convertToRange(godot::Variant var) {
		switch (var.get_type()) {
		case VType::INT:
			return int(var);
		case VType::REAL:
			return double(var);
		case VType::BOOL:
			return bool(var);
		default:
			return std::nullopt;
		}
	}
	std::optional<ValueDef> convertToValueDef(godot::Variant var) {
		if (var.get_type() != VType::DICTIONARY) {
			return std::nullopt;
		}
		godot::Dictionary dict = var;
		godot::Variant typevar = dict["type"];
		godot::Variant countvar = dict["count"];
		godot::Variant rangevar = dict["range"];
		
		gdev::ValueType type;
		{
			auto typeopt = convertToValueType(typevar);
			if (!typeopt) {
				Godot::print("'type' entry in dictionary was not a valid type for ValueDef!");
				return std::nullopt;
			}
			type = typeopt.value();
		}

		int count = 1;
		if (countvar.get_type() == VType::INT) {
			int tmp = countvar;
			if (tmp > 1) {
				count = tmp;
			}
		}
		else if (countvar.get_type() != VType::NIL) {
			Godot::print("Entry in dictionary has non-integer 'count'!");
			return std::nullopt;
		}
		
		double low = 0.0, high = 1.0;
		if (rangevar.get_type() == VType::ARRAY) {
			godot::Array rangearr = rangevar;
			if (rangearr.size() != 2) {
				return std::nullopt;
			}

			auto lowopt = convertToRange(rangearr[0]);
			auto highopt = convertToRange(rangearr[1]);

			if (lowopt && highopt) {
				low = lowopt.value();
				high = highopt.value();
			}
			else {
				return std::nullopt;
			}
		}
		else if (rangevar.get_type() != VType::NIL) {
			return std::nullopt;
		}
		else {
			switch (type) {
			case gdev::ValueType::Bool:
				low = 0.0;
				high = 1.0;
				break;
			case gdev::ValueType::Real:
				low = -std::numeric_limits<double>::infinity();
				high = std::numeric_limits<double>::infinity();
				break;
			case gdev::ValueType::Int:
				low = std::numeric_limits<int>::min();
				high = std::numeric_limits<int>::max();
				break;
			default:
				break;
			}
		}

		switch (type) {
		case ValueType::Bool:
			return ValueDef::MakeBinary({count, 1, 1, 1});
		case ValueType::Int:
			return ValueDef::MakeCategorical({ count, 1, 1, 1 }, low, high);
		case ValueType::Real:
			return ValueDef::MakeReal({ count, 1, 1, 1 }, low, high);
		default:
			return std::nullopt;
		}
	}
	
	template<std::size_t N>
	constexpr std::string_view makeView(const char (&arr)[N]) {
		return std::string_view(arr, N-1);
	}

	std::string_view to_string_view(godot::Variant::Type type) {
		switch (type) {
		default:
		case VType::NIL:
			return makeView("NIL");
		case VType::BOOL:
			return makeView("BOOL");
		case VType::INT:
			return makeView("INT");
		case VType::REAL:
			return makeView("REAL");
		case VType::STRING:
			return makeView("STRING");
		case VType::VECTOR2:
			return makeView("VECTOR2");
		case VType::RECT2:
			return makeView("RECT2");
		case VType::VECTOR3:
			return makeView("VECTOR3");
		case VType::TRANSFORM2D:
			return makeView("TRANSFORM2D");
		case VType::PLANE:
			return makeView("PLANE");
		case VType::QUAT:
			return makeView("QUAT");
		case VType::RECT3:
			return makeView("RECT3");
		case VType::BASIS:
			return makeView("BASIS");
		case VType::TRANSFORM:
			return makeView("TRANSFORM");
		case VType::COLOR:
			return makeView("COLOR");
		case VType::NODE_PATH:
			return makeView("NODE_PATH");
		case VType::_RID:
			return makeView("_RID");
		case VType::OBJECT:
			return makeView("OBJECT");
		case VType::DICTIONARY:
			return makeView("DICTIONARY");
		case VType::ARRAY:
			return makeView("ARRAY");
		case VType::POOL_BYTE_ARRAY:
			return makeView("POOL_BYTE_ARRAY");
		case VType::POOL_INT_ARRAY:
			return makeView("POOL_INT_ARRAY");
		case VType::POOL_REAL_ARRAY:
			return makeView("POOL_REAL_ARRAY");
		case VType::POOL_STRING_ARRAY:
			return makeView("POOL_STRING_ARRAY");
		case VType::POOL_VECTOR2_ARRAY:
			return makeView("POOL_VECTOR2_ARRAY");
		case VType::POOL_VECTOR3_ARRAY:
			return makeView("POOL_VECTOR3_ARRAY");
		case VType::POOL_COLOR_ARRAY:
			return makeView("POOL_COLOR_ARRAY");
		}
	}
}