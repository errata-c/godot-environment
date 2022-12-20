#pragma once
#include <cinttypes>
#include <string>
#include <gdev/ValueType.hpp>
#include <gdev/RequestType.hpp>

namespace gdev {
	class Value;
	class Space;


	void serialize(const gdev::Value& value, std::string& buffer);
	void serialize(const gdev::Space& value, std::string& buffer);

	void serializeStep(const gdev::Space& observation, float reward, bool done, std::string& buffer);


	const char* deserialize(const char* buffer, const char* end, Value& value);
	const char* deserialize(const char* buffer, const char* end, Space& value);

	const char* deserializeStep(const char* buffer, const char* end, gdev::Space& observation, float &reward, bool &done);
}