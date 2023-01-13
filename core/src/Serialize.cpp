#include <gdev/Serialize.hpp>
#include <gdev/Value.hpp>
#include <gdev/Space.hpp>

#include <ez/serialize/core.hpp>
#include <ez/serialize/strings.hpp>

namespace gdev {
	struct _serialize_func {
		template<typename T>
		static void apply(const Value& _val, std::string& buffer) {
			using ez::serialize::value;

			auto it = (const T*)_val.data();
			auto last = it + _val.elements();
			while (it != last) {
				value(*it++, buffer);
			}
		}
	};
	void serialize(const Value& value, std::string& buffer) {
		ez::serialize::enumerator(value.type(), buffer);
		ez::serialize::u64(value.dim(0), buffer);
		ez::serialize::u64(value.dim(1), buffer);
		ez::serialize::u64(value.dim(2), buffer);
		ez::serialize::u64(value.dim(3), buffer);

		visitors::single_visit<_serialize_func>(value.type(), value, buffer);
	}

	struct _deserialize_func {
		template<typename T>
		static const char* apply(Value& _val, const char* buffer, const char* buffer_end) {
			using ez::deserialize::value;

			auto it = (T*)_val.data();
			auto end = it + _val.elements();

			for (; it != end; ++it) {
				buffer = value(buffer, buffer_end, *it);
			}
			return buffer;
		}
	};
	const char* deserialize(const char* buffer, const char* end, Value& value) {
		ValueType type;
		buffer = ez::deserialize::enumerator(buffer, end, type);

		// Just in case std::size_t is not u64
		std::array<uint64_t, 4> tmp;
		dims_t dims;
		buffer = ez::deserialize::u64(buffer, end, tmp[0]);
		buffer = ez::deserialize::u64(buffer, end, tmp[1]);
		buffer = ez::deserialize::u64(buffer, end, tmp[2]);
		buffer = ez::deserialize::u64(buffer, end, tmp[3]);
		for (int i = 0; i < 4; ++i) {
			dims[i] = tmp[i];
		}

		value = Value::Uninitialized(dims, type);

		buffer = visitors::single_visit<_deserialize_func>(type, value, buffer, end);

		return buffer;
	}


	void serialize(const Space& value, std::string& buffer) {
		ez::serialize::u64(value.size(), buffer);

		for (const auto & subvalue : value) {
			ez::serialize::string(subvalue.first, buffer);
			serialize(subvalue.second, buffer);
		}
	}
	const char* deserialize(const char* buffer, const char* end, Space& space) {
		uint64_t count;
		buffer = ez::deserialize::u64(buffer, end, count);

		for (uint64_t i = 0; i < count; ++i) {
			std::string name;
			Value subvalue;
			buffer = ez::deserialize::string(buffer, end, name);
			buffer = deserialize(buffer, end, subvalue);

			space.insert_or_assign(std::move(name), std::move(subvalue));
		}

		return buffer;
	}


	
	void serializeStep(const gdev::Space& observation, float reward, bool done, std::string& buffer) {
		ez::serialize::u8(uint8_t(done), buffer);
		ez::serialize::f32(reward, buffer);
		serialize(observation, buffer);
	}
	
	const char* deserializeStep(const char* buffer, const char* end, gdev::Space& observation, float& reward, bool& done) {
		uint8_t donetmp;
		buffer = ez::deserialize::u8(buffer, end, donetmp);
		done = donetmp > 0;

		buffer = ez::deserialize::f32(buffer, end, reward);
		return deserialize(buffer, end, observation);
	}
	
}