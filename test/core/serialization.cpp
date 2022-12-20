#ifdef _MSC_BUILD
#pragma warning(disable: 26812)
#endif

#include <catch2/catch_all.hpp>
#include <gdev/Value.hpp>
#include <gdev/Space.hpp>

#include <gdev/Serialize.hpp>


TEST_CASE("serialize values") {
	std::vector<gdev::Value> values{{
		gdev::Value(true),
		gdev::Value(0.5),
		gdev::Value(2),

		gdev::Value(true, gdev::dim_t{10,1,1,1}),
		gdev::Value(0.5, gdev::dim_t{10,1,1,1}),
		gdev::Value(2, gdev::dim_t{10,1,1,1})
	}};

	std::string buffer;
	for (gdev::Value & val : values) {
		gdev::serialize(val, buffer);
	}
	
	std::vector<gdev::Value> regen(values.size(), gdev::Value{});
	
	const char* data = buffer.data(), * end = buffer.data() + buffer.size();
	for (gdev::Value & value: regen) {
		const char* prev = data;

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
	space.insert(std::make_pair("bool", true));
	space.insert(std::make_pair("real", 0.0));
	space.insert(std::make_pair("cat", 0));

	std::string buffer;
	gdev::serialize(space, buffer);

	const char* data = buffer.data(), *end = buffer.data() + buffer.size();

	gdev::Space regen;
	data = gdev::deserialize(data, end, regen);

	REQUIRE(space == regen);
}