#include <gdev/GodotConversions.hpp>

#include <gdev/ValueWrapper.hpp>
#include <gdev/Serialize.hpp>

#include <Dictionary.hpp>
#include <String.hpp>
#include <Godot.hpp>

#include <iostream>
#include <unordered_map>
#include <limits>

#include <ez/serialize/core.hpp>
#include <ez/serialize/ranges.hpp>
#include <ez/serialize/strings.hpp>

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

	std::optional<ValueType> to_value_type(godot::Variant var) {
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

	std::optional<range_t> to_range(godot::Variant var) {
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

	std::optional<dims_t> to_dims(godot::Variant var) {
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


	godot::Variant to_variant(const gdev::ValueType& val) {
		return godot::String(gdev::to_string_view(val).data());
	}
	godot::Variant to_variant(const gdev::range_t& val) {
		// Default to a dictionary
		godot::Dictionary dict;
		dict["low"] = val[0];
		dict["high"] = val[1];
		return dict;
	}
	godot::Variant to_variant(const gdev::dims_t& val) {
		godot::Array arr;
		arr.resize(4);
		arr[0] = val[0];
		arr[1] = val[1];
		arr[2] = val[2];
		arr[3] = val[3];
		return arr;
	}
	
	void serialize_space(const godot::Dictionary& space, std::string& buffer) {
		using namespace godot;

		Array& keys = space.keys();
		Array& values = space.values();

		size_t count = static_cast<size_t>(keys.size());

		// Verify that everything is correct first.
		for (size_t i = 0; i < count; ++i) {
			Variant& k = keys[i];
			Variant& v = values[i];

			if (v.get_type() != Variant::OBJECT) {
				ERR_PRINT("Expected the space dictionary to contain ONLY ValueWrapper elements");
				return;
			}
			Object * obj(v);
			if (typeid(*obj).hash_code() != ValueWrapper::___get_id()) {
				ERR_PRINT("Expected the space dictionary to contain ONLY ValueWrapper elements");
				return;
			}
			ValueWrapper * wrapped = static_cast<ValueWrapper*>(obj);

			if (k.get_type() != Variant::STRING) {
				ERR_PRINT("A space can only have string keys for now");
				return;
			}
		}

		ez::serialize::u64(count, buffer);

		for (size_t i = 0; i < count; ++i) {
			Variant& k = keys[i];
			Variant& v = values[i];

			String key = k.operator String();
			ValueWrapper* wrapped = static_cast<ValueWrapper*>((Object*)(v));

			CharString utf8 = key.utf8();

			ez::serialize::u64(uint64_t(utf8.length()), buffer);
			ez::serialize::range(utf8.get_data(), utf8.get_data() + utf8.length(), buffer);

			serialize(wrapped->data, buffer);
		}
	}
	const char* deserialize_space(const char* buffer, const char* end, godot::Dictionary& space) {
		// Deserialize in the same manner as the c++ function

		uint64_t count;
		buffer = ez::deserialize::u64(buffer, end, count);

		std::string key;
		

		for (uint64_t i = 0; i < count; ++i) {
			Value value;

			buffer = ez::deserialize::string(buffer, end, key);
			buffer = deserialize(buffer, end, value);

			auto wrapped = godot::ValueWrapper::_new();
			wrapped->data = std::move(value);

			space[key.c_str()] = wrapped;
		}

		return buffer;
	}
}