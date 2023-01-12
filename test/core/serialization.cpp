#ifdef _MSC_BUILD
#pragma warning(disable: 26812)
#endif

#include <catch2/catch_all.hpp>
#include <gdev/Value.hpp>
#include <gdev/Space.hpp>

#include <gdev/Serialize.hpp>


TEST_CASE("serialize values") {
	auto value = GENERATE(
		gdev::Value(true),
		gdev::Value(0.5),
		gdev::Value(2),

		gdev::Value(gdev::dims_t{ 10,1,1,1 }, true),
		gdev::Value(gdev::dims_t{ 10,1,1,1 }, 0.5),
		gdev::Value(gdev::dims_t{ 10,1,1,1 }, 2)
	);

	std::string buffer;
	gdev::serialize(value, buffer);

	gdev::Value regen;
	
	const char* data = buffer.data();
	const char* end = buffer.data() + buffer.size();

	const char * read_end = gdev::deserialize(data, end, regen);
	REQUIRE((read_end - data) == (end - data));

	REQUIRE(value == regen);
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