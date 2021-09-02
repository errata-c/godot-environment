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
		else if (typestring == "int") {
			return gdev::ValueType::Int;
		}
		else if (typestring == "bool") {
			return gdev::ValueType::Bool;
		}
		else {
			return std::nullopt;
		}
	}

	std::optional<range_t> convertToRange(godot::Variant var) {
		std::array<godot::Variant, 2> grange;
		range_t range;

		switch (var.get_type()) {
		case VType::ARRAY: {
			godot::Array arr = var;
			if (arr.size() != 2) {
				return std::nullopt;
			}
			grange[0] = arr[0];
			grange[1] = arr[1];
			break;
		}
		case VType::DICTIONARY: {
			godot::Dictionary dict = var;
			grange[0] = dict["low"];
			grange[1] = dict["high"];
			break;
		}
		default:
			return std::nullopt;
		}

		for (int i = 0; i < 2; ++i) {
			switch (grange[i].get_type()) {
			case VType::INT:
				range[i] = int(grange[i]);
				break;
			case VType::REAL:
				range[i] = double(grange[i]);
				break;
			case VType::BOOL:
				range[i] = bool(grange[i]);
				break;
			default:
				return std::nullopt;
			}
		}

		return range;
	}

	std::optional<dim_t> convertToDims(godot::Variant var) {
		switch (var.get_type()) {
		case VType::INT:
			return dim_t{int(var), 1, 1, 1};
		case VType::ARRAY: {
			godot::Array arr = var;
			if (arr.size() <= 4) {
				dim_t res{1,1,1,1};

				for (int i = 0; i < arr.size(); ++i) {
					godot::Variant element = arr[i];
					if (element.get_type() != VType::INT) {
						return std::nullopt;
					}
					else {
						res[i] = int(element);
					}
				}

				return res;
			}
			else {
				return std::nullopt;
			}
		}
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
		godot::Variant dimsvar = dict["dims"];
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

		dim_t dims{ 1,1,1,1 };
		if (dimsvar.get_type() != VType::NIL) {
			auto tmp = convertToDims(dimsvar);
			if (tmp) {
				dims = tmp.value();
			}
			else {
				return std::nullopt;
			}
		}
		
		range_t range;
		if (rangevar.get_type() != VType::NIL) {
			auto tmp = convertToRange(rangevar);
			if (tmp) {
				range = tmp.value();
			}
			else {
				return std::nullopt;
			}
		}
		else {
			switch (type) {
			default:
			case gdev::ValueType::Bool:
				range = ValueDef::BoolRange();
				break;
			case gdev::ValueType::Real:
				range = ValueDef::RealRange();
				break;
			case gdev::ValueType::Int:
				range = ValueDef::IntRange();
				break;
			}
		}

		switch (type) {
		case ValueType::Bool:
			return ValueDef::MakeBool(dims);
		case ValueType::Int:
			return ValueDef::MakeInt(dims, range);
		case ValueType::Real:
			return ValueDef::MakeReal(dims, range);
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

	godot::Variant convertToVariant(const gdev::Value& value) {
		std::size_t size = value.size();
		if (size == 1) {
			switch (value.type()) {
			default:
			case gdev::ValueType::Bool:
				return godot::Variant(value.asBool());
			case gdev::ValueType::Int:
				return godot::Variant(value.asInt());
			case gdev::ValueType::Real:
				return godot::Variant(value.asReal());
			}
		}
		else {
			godot::Array arr;
			arr.resize(size);

			switch (value.type()) {
			case ValueType::Bool: {
				auto it = (const bool*)value.begin();
				for (std::size_t i = 0; i < size; ++i) {
					arr[i] = *it;
					++it;
				}
				break;
			}
			case ValueType::Int: {
				auto it = (const int*)value.begin();
				for (std::size_t i = 0; i < size; ++i) {
					arr[i] = *it;
					++it;
				}
				break;
			}
			case ValueType::Real: {
				auto it = (const double*)value.begin();
				for (std::size_t i = 0; i < size; ++i) {
					arr[i] = *it;
					++it;
				}
				break;
			}}
			
			return arr;
		}
	}
}