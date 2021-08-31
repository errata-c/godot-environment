#include <gdev/Serialize.hpp>
#include <gdev/Value.hpp>
#include <gdev/ValueDef.hpp>
#include <gdev/Space.hpp>
#include <gdev/SpaceDef.hpp>

#include <ez/serialize.hpp>
#include <ez/deserialize.hpp>

namespace gdev {
	void serialize(SerializedType type, std::vector<uint8_t>& buffer) {
		ez::serialize::u8((uint8_t)type, buffer);
	}
	void serialize(ValueType type, std::vector<uint8_t>& buffer) {
		ez::serialize::u8((uint8_t)type, buffer);
	}

	void serialize(const Value& value, std::vector<uint8_t>& buffer) {
		serialize(SerializedType::Value, buffer);
		serialize(value.type(), buffer);
		ez::serialize::i32(value.dim(0), buffer);
		ez::serialize::i32(value.dim(1), buffer);
		ez::serialize::i32(value.dim(2), buffer);
		ez::serialize::i32(value.dim(3), buffer);

		switch (value.type()) {
		case ValueType::Bool: {
			auto it = (const bool*)value.begin();
			auto last = (const bool*)value.end();
			for (; it != last; ++it) {
				ez::serialize::u8((uint8_t)*it, buffer);
			}
			break;
		}
		case ValueType::Int: {
			auto it = (const int*)value.begin();
			auto last = (const int*)value.end();
			for (; it != last; ++it) {
				ez::serialize::i32((int32_t)*it, buffer);
			}
			break;
		}
		case ValueType::Real: {
			auto it = (const double*)value.begin();
			auto last = (const double*)value.end();
			for (; it != last; ++it) {
				ez::serialize::f64(*it, buffer);
			}
			break;
		}}
	}
	void serialize(const ValueDef& value, std::vector<uint8_t>& buffer) {
		serialize(SerializedType::ValueDef, buffer);
		serialize(value.type(), buffer);
		ez::serialize::i32(value.dim(0), buffer);
		ez::serialize::i32(value.dim(1), buffer);
		ez::serialize::i32(value.dim(2), buffer);
		ez::serialize::i32(value.dim(3), buffer);

		switch (value.type()) {
		case ValueType::Bool: {
			break;
		}
		case ValueType::Int: {
			ez::serialize::i32(value.lowerBound(), buffer);
			ez::serialize::i32(value.upperBound(), buffer);
			break;
		}
		case ValueType::Real: {
			ez::serialize::f64(value.lowerBound(), buffer);
			ez::serialize::f64(value.upperBound(), buffer);
			break;
		}}
	}
	void serialize(const Space& value, std::vector<uint8_t>& buffer) {
		serialize(SerializedType::Space, buffer);
		ez::serialize::u64(value.size(), buffer);

		for (const auto & subvalue : value) {
			ez::serialize::string(subvalue.name, buffer);
			serialize(subvalue.value, buffer);
		}
	}
	void serialize(const SpaceDef& value, std::vector<uint8_t>& buffer) {
		serialize(SerializedType::SpaceDef, buffer);
		ez::serialize::u64(value.size(), buffer);

		for (const auto& subvalue : value) {
			ez::serialize::string(subvalue.name, buffer);
			serialize(subvalue.value, buffer);
		}
	}

	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, SerializedType& value) {
		uint8_t tmp;
		buffer = ez::deserialize::u8(buffer, end, tmp);
		value = static_cast<SerializedType>(tmp);
		return buffer;
	}
	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, ValueType& value) {
		uint8_t tmp;
		buffer = ez::deserialize::u8(buffer, end, tmp);
		value = static_cast<ValueType>(tmp);
		return buffer;
	}

	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, Value& value) {
		ValueType type;
		buffer = deserialize(buffer, end, type);

		dim_t dims;
		buffer = ez::deserialize::i32(buffer, end, dims[0]);
		buffer = ez::deserialize::i32(buffer, end, dims[1]);
		buffer = ez::deserialize::i32(buffer, end, dims[2]);
		buffer = ez::deserialize::i32(buffer, end, dims[3]);

		switch (type) {
		case ValueType::Bool: {
			value = Value::MakeBool(false, dims[0], dims[1], dims[2], dims[3]);

			auto it = (bool*)value.begin();
			auto last = (bool*)value.end();

			for (; it != last; ++it) {
				uint8_t bin;
				buffer = ez::deserialize::u8(buffer, end, bin);
				*it = bin;
			}
			break;
		}
		case ValueType::Int: {
			value = Value::MakeInt(0, dims[0], dims[1], dims[2], dims[3]);

			auto it = (int*)value.begin();
			auto last = (int*)value.end();

			for (; it != last; ++it) {
				int32_t bin;
				buffer = ez::deserialize::i32(buffer, end, bin);
				*it = bin;
			}
			break;
		}
		case ValueType::Real: {
			value = Value::MakeReal(0.0, dims[0], dims[1], dims[2], dims[3]);

			auto it = (double*)value.begin();
			auto last = (double*)value.end();

			for (; it != last; ++it) {
				double bin;
				buffer = ez::deserialize::f64(buffer, end, bin);
				*it = bin;
			}
			break;
		}}

		return buffer;
	}
	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, ValueDef& value) {
		ValueType type;
		buffer = deserialize(buffer, end, type);

		dim_t dims;
		buffer = ez::deserialize::i32(buffer, end, dims[0]);
		buffer = ez::deserialize::i32(buffer, end, dims[1]);
		buffer = ez::deserialize::i32(buffer, end, dims[2]);
		buffer = ez::deserialize::i32(buffer, end, dims[3]);

		switch (type) {
		case ValueType::Bool:
			value = ValueDef::MakeBool(dims);
			break;
		case ValueType::Int: {
			int32_t low, high;
			buffer = ez::deserialize::i32(buffer, end, low);
			buffer = ez::deserialize::i32(buffer, end, high);

			value = ValueDef::MakeInt(dims, low, high);
			break;
		}
		case ValueType::Real: {
			double low, high;
			buffer = ez::deserialize::f64(buffer, end, low);
			buffer = ez::deserialize::f64(buffer, end, high);

			value = ValueDef::MakeReal(dims, low, high);
			break;
		}}

		return buffer;
	}
	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, Space& space) {
		uint64_t count;
		buffer = ez::deserialize::u64(buffer, end, count);

		for (uint64_t i = 0; i < count; ++i) {
			std::string name;
			Value subvalue;
			buffer = ez::deserialize::string(buffer, end, name);
			buffer = deserialize(buffer, end, subvalue);

			space.insert(std::move(name), std::move(subvalue));
		}

		return buffer;
	}
	const uint8_t* deserialize(const uint8_t* buffer, const uint8_t* end, SpaceDef& space) {
		uint64_t count;
		buffer = ez::deserialize::u64(buffer, end, count);

		for (uint64_t i = 0; i < count; ++i) {
			std::string name;
			ValueDef subvalue;
			buffer = ez::deserialize::string(buffer, end, name);
			buffer = deserialize(buffer, end, subvalue);

			space.insert(std::move(name), std::move(subvalue));
		}

		return buffer;
	}
}