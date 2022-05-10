#pragma once
#include <cinttypes>
#include <string>
#include "ValueType.hpp"
#include "RequestType.hpp"

namespace gdev {
	class Value;
	class ValueDef;
	class Space;
	class SpaceDef;

	void serialize(RequestType type, std::string& buffer);
	void serialize(ValueType type, std::string& buffer);

	void serialize(const gdev::Value& value, std::string& buffer);
	void serialize(const gdev::ValueDef& value, std::string& buffer);
	void serialize(const gdev::Space& value, std::string& buffer);
	void serialize(const gdev::SpaceDef& value, std::string& buffer);

	void serializeDefs(const gdev::SpaceDef& acSpace, const gdev::SpaceDef& obSpace, std::string& buffer);
	void serializeStep(const gdev::Space& observation, float reward, bool done, std::string& buffer);

	const char* deserialize(const char* buffer, const char* end, RequestType& value);
	const char* deserialize(const char* buffer, const char* end, ValueType& value);

	const char* deserialize(const char* buffer, const char* end, Value& value);
	const char* deserialize(const char* buffer, const char* end, ValueDef& value);
	const char* deserialize(const char* buffer, const char* end, Space& value);
	const char* deserialize(const char* buffer, const char* end, SpaceDef& value);

	const char* deserializeDefs(const char* buffer, const char* end, gdev::SpaceDef& acSpace, gdev::SpaceDef& obSpace);
	const char* deserializeStep(const char* buffer, const char* end, gdev::Space& observation, float &reward, bool &done);
}