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

	void serialize(RequestType type, std::vector<uint8_t>& buffer);
	void serialize(ValueType type, std::vector<uint8_t>& buffer);

	void serialize(const gdev::Value& value, std::vector<uint8_t>& buffer);
	void serialize(const gdev::ValueDef& value, std::vector<uint8_t>& buffer);
	void serialize(const gdev::Space& value, std::vector<uint8_t>& buffer);
	void serialize(const gdev::SpaceDef& value, std::vector<uint8_t>& buffer);

	void serializeDefs(const gdev::SpaceDef& acSpace, const gdev::SpaceDef& obSpace, std::vector<uint8_t>& buffer);
	void serializeStep(const gdev::Space& observation, float reward, bool done, std::vector<uint8_t>& buffer);

	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, RequestType& value);
	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, ValueType& value);

	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, Value& value);
	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, ValueDef& value);
	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, Space& value);
	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, SpaceDef& value);

	const uint8_t* deserializeDefs(const uint8_t * buffer, const uint8_t* end, gdev::SpaceDef& acSpace, gdev::SpaceDef& obSpace);
	const uint8_t* deserializeStep(const uint8_t* buffer, const uint8_t* end, gdev::Space& observation, float &reward, bool &done);
}