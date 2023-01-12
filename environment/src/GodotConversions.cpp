#include <gdev/GodotConversions.hpp>

#include <Dictionary.hpp>
#include <String.hpp>
#include <Godot.hpp>

#include <iostream>
#include <unordered_map>
#include <limits>

using VType = godot::Variant::Type;
using Godot = godot::Godot;

namespace gdev {
	template<std::size_t N>
	constexpr std::string_view makeView(const char(&arr)[N]) {
		return std::string_view(arr, N - 1);
	}

	// I can't for the life of me find a way to convert this to a string using gdnative, so I have to make my own function.
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

	godot::String to_godot_string(godot::Variant::Type vtype) {
		return godot::String(to_string_view(vtype).data());
	}

	std::optional<ValueType> convertToValueType(godot::Variant var) {
		// consider some mechanism for localization?

		// Make sure its a string first
		if (var.get_type() != VType::STRING) {
			WARN_PRINT("Expected a string!");
			return std::nullopt;
		}

		// Get the string object itself.
		godot::String typestring = var;

		// Must have at least one character.
		if (typestring.length() < 1) {
			WARN_PRINT("String was empty!");
			return std::nullopt;
		}

		// Check the first character
		switch (typestring[0]) {
		case 'b':
			if (typestring.length() == 2 && typestring[1] == '8') {
				return gdev::ValueType::b8;
			}
			else {
				break;
			}

		case 'i':
			switch (typestring[1]) {
			case '8':
				if (typestring.length() == 2) {
					return gdev::ValueType::i8;
				}
				else {
					break;
				}
			case '1':
				if (typestring.length() == 3 && typestring[2] == '6') {
					return gdev::ValueType::i16;
				}
				else {
					break;
				}
			case '3':
				if (typestring.length() == 3 && typestring[2] == '2') {
					return gdev::ValueType::i32;
				}
				else {
					break;
				}
			case '6':
				if (typestring.length() == 3 && typestring[2] == '4') {
					return gdev::ValueType::i64;
				}
				else {
					break;
				}
			default:
				break;
			}
			break;

		case 'f':
			if (typestring[1] == '3' && typestring[2] == '2' && typestring.length() == 3) {
				// f32
				return gdev::ValueType::f32;
			}
			else if (typestring[1] == '6' && typestring[2] == '4' && typestring.length() == 3) {
				// f64
				return gdev::ValueType::f64;
			}
			else {
				break;
			}

		default:
			break;
		}

		WARN_PRINT(
			godot::String("'{0}' is not a valid vtype!").format(godot::Array::make(var))
		);
		return std::nullopt;
	}

	std::optional<range_t> convertToRange(godot::Variant var) {
		range_t range;

		switch (var.get_type()) {
		case VType::ARRAY: {
			godot::Array arr = var;
			if (arr.size() != 2) {
				WARN_PRINT("Expected array to have two elements!");
				return std::nullopt;
			}
			auto lv = convert_to<double>(arr[0]);
			auto hv = convert_to<double>(arr[1]);

			if (!lv || !hv) {
				WARN_PRINT("Failed to convert array to a valid range!");
				return std::nullopt;
			}

			range[0] = *lv;
			range[1] = *hv;
			break;
		}
		case VType::DICTIONARY: {
			godot::Dictionary dict = var;
			if (!dict.has(godot::String("low"))) {
				WARN_PRINT("Expected dictionary to have a 'low' value!");
				return std::nullopt;
			}
			if (!dict.has(godot::String("high"))) {
				WARN_PRINT("Expected dictionary to have a 'high' value!");
				return std::nullopt;
			}

			auto lv = convert_to<double>(dict["low"]);
			auto hv = convert_to<double>(dict["high"]);

			if (!lv || !hv) {
				WARN_PRINT("Failed to convert dictionary to a valid range!");
				return std::nullopt;
			}

			range[0] = *lv;
			range[1] = *hv;
			break;
		}
		default:
			
			WARN_PRINT("Expected an Array or Dictionary!");
			return std::nullopt;
		}

		return range;
	}

	std::optional<dims_t> convertToDims(godot::Variant var) {
		switch (var.get_type()) {
		case VType::INT: {
			int64_t d(var);
			if (d <= 0) {
				WARN_PRINT("Dimension given was less than 1!");
				return std::nullopt;
			}
			else {
				return dims_t{ static_cast<size_t>(d), 1, 1, 1 };
			}
		}
		case VType::ARRAY: {
			godot::Array arr = var;
			if (arr.size() <= 4 && arr.size() > 0) {
				dims_t res{1,1,1,1};

				for (int i = 0; i < arr.size(); ++i) {
					godot::Variant & element = arr[i];
					if (element.get_type() != VType::INT) {
						
						WARN_PRINT(
							godot::String("Dimension {0} was not an integer!").format(godot::Array::make(i))
						);
						return std::nullopt;
					}
					else {
						int64_t d(element);
						if (d <= 0) {
							WARN_PRINT(
								godot::String("Dimension {0} was less than 1!").format(godot::Array::make(i))
								);
							return std::nullopt;
						}
						else {
							res[i] = static_cast<size_t>(d);
						}
					}
				}

				return res;
			}
			else {
				if (arr.size() == 0) {
					WARN_PRINT("Dimensions array was empty!");
				}
				else {
					WARN_PRINT("Dimensions array had too many elements!");
				}
				return std::nullopt;
			}
		}
		default:
			WARN_PRINT(
				godot::String("Cannot convert {0} to dims!").format(godot::Array::make(var))
			);
			return std::nullopt;
		}
	}


	std::optional<Value> convertToValue(godot::Variant var) {
		switch (var.get_type()) {
		case VType::BOOL: {
			return Value(bool(var));
		}
		case VType::INT: {
			return Value(int(var));
		}
		case VType::REAL: {
			return Value(double(var));
		}
		case VType::ARRAY: {
			// Check the size of the array, then go element by element, checking the types of the subelements
			// Make sure that the array is either multidimensional, or that all elements are the same ValueType
			godot::Array arr = var;
			std::size_t size = arr.size();
			if (size == 0) {
				// Empty array is not allowed
				Godot::print("Array passed into convertToValue was empty!");
				return std::nullopt;
			}

			gdev::Value result;

			VType etype = arr[0].get_type();
			switch (etype) {
			case VType::BOOL:
				result = gdev::Value(size, false);
				break;
			case VType::INT:
				result = gdev::Value(size, 0);
				break;
			case VType::REAL:
				result = gdev::Value(size, 0.0);
				break;
			default:
				// Elements must be a valid type
				Godot::print("Array passed into convertToValue did not have a valid element type!");
				return std::nullopt;
			}

			for (std::size_t i = 0; i < size; ++i) {
				if (arr[i].get_type() != etype) {
					Godot::print("Array passed into convertToValue did not have elements all of the same type!");
					return std::nullopt;
				}
				switch (etype) {
				case VType::BOOL:
					result[i] = bool(arr[i]);
					break;
				case VType::INT:
					result[i] = int(arr[i]);
					break;
				case VType::REAL:
					result[i] = double(arr[i]);
					break;
				}
			}

			return result;
		}
		default:
			Godot::print("Variant passed into convertToValue was not a supported type!");
			return std::nullopt;
		}
	}


	godot::Variant convertToVariant(const gdev::ValueType& val) {
		return godot::String(gdev::to_string_view(val).data());
	}
	godot::Variant convertToVariant(const gdev::range_t& val) {
		// Default to a dictionary
		godot::Dictionary dict;
		dict["low"] = val[0];
		dict["high"] = val[1];
		return dict;
	}
	godot::Variant convertToVariant(const gdev::dims_t& val) {
		godot::Array arr;
		arr.resize(4);
		arr[0] = val[0];
		arr[1] = val[1];
		arr[2] = val[2];
		arr[3] = val[3];
		return arr;
	}
}