#pragma once
#include <cinttypes>
#include <vector>
#include "ValueType.hpp"
#include "RequestType.hpp"

namespace gdev {
	class Value;
	class ValueDef;
	class Space;
	class SpaceDef;

	enum class SerializedType: uint8_t {
		Value = 0,
		ValueDef,
		Space,
		SpaceDef,
	};

	void serialize(RequestType type, std::vector<uint8_t>& buffer);
	void serialize(SerializedType type, std::vector<uint8_t>& buffer);
	void serialize(ValueType type, std::vector<uint8_t>& buffer);

	void serialize(const gdev::Value& value, std::vector<uint8_t>& buffer);
	void serialize(const gdev::ValueDef& value, std::vector<uint8_t>& buffer);
	void serialize(const gdev::Space& value, std::vector<uint8_t>& buffer);
	void serialize(const gdev::SpaceDef& value, std::vector<uint8_t>& buffer);

	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, RequestType& value);
	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, SerializedType& value);
	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, ValueType& value);

	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, Value& value);
	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, ValueDef& value);
	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, Space& value);
	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, SpaceDef& value);
}