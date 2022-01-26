#ifdef _MSC_BUILD
#pragma warning(disable: 26812)
#endif

#include <catch2/catch.hpp>
#include <gdev/Value.hpp>
#include <gdev/ValueDef.hpp>
#include <gdev/Space.hpp>
#include <gdev/SpaceDef.hpp>

#include <gdev/Serialize.hpp>

TEST_CASE("serialize values") {
	std::vector<gdev::Value> values{{
		gdev::Value::MakeBool(true),
		gdev::Value::MakeReal(0.5),
		gdev::Value::MakeInt(2),

		gdev::Value::MakeBool(true, 10),
		gdev::Value::MakeReal(0.5, 10),
		gdev::Value::MakeInt(2, 10)
	}};

	std::vector<uint8_t> buffer;
	for (gdev::Value & val : values) {
		gdev::serialize(val, buffer);
	}
	
	std::vector<gdev::Value> regen(values.size(), gdev::Value{});
	
	const uint8_t* data = buffer.data(), * end = buffer.data() + buffer.size();
	for (gdev::Value & value: regen) {
		const uint8_t * prev = data;

		REQUIRE_NOTHROW(data = gdev::deserialize(data, end, value));
		REQUIRE(prev != data);
	}

	REQUIRE(values.size() == regen.size());

	for (std::size_t i = 0; i < values.size(); ++i) {
		REQUIRE(values[i] == regen[i]);
	}
}

TEST_CASE("Serialize space") {
	gdev::Space space;
	space.insert("bool", gdev::Value::MakeBool(true));
	space.insert("real", gdev::Value::MakeReal(0.5f));
	space.insert("cat", gdev::Value::MakeInt(2));

	std::vector<uint8_t> buffer;
	gdev::serialize(space, buffer);

	const uint8_t * data = buffer.data(), *end = buffer.data() + buffer.size();

	gdev::Space regen;
	data = gdev::deserialize(data, end, regen);

	REQUIRE(space == regen);
}

TEST_CASE("Serialize space def") {
	gdev::SpaceDef space;
	space.insert("bool", gdev::ValueDef::MakeBool(gdev::dim_t{ 1,2,3,4 }));
	space.insert("real", gdev::ValueDef::MakeBool(gdev::dim_t{ 2,3,4,5 }));
	space.insert("cat", gdev::ValueDef::MakeBool(gdev::dim_t{ 3,4,5,6 }));

	std::vector<uint8_t> buffer;
	gdev::serialize(space, buffer);
	const uint8_t* data = buffer.data(), * end = buffer.data() + buffer.size();

	gdev::SpaceDef regen;
	data = gdev::deserialize(data, end, regen);

	REQUIRE(space == regen);
}