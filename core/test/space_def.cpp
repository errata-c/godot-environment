#ifdef _MSC_BUILD
#pragma warning(disable: 26812)
#endif

#include <catch.hpp>
#include <gdev/SpaceDef.hpp>

using namespace gdev;

TEST_CASE("SpaceDef initialization", "[space]") {
	SpaceDef space;

	REQUIRE(space.size() == 0);
	REQUIRE(space.empty());

	space.insert("one", ValueDef::MakeBinary({1,1,1,1}));
	space.insert("two", ValueDef::MakeReal({1,1,1,1}));
	space.insert("three", ValueDef::MakeCategorical({1,1,1,1}));

	REQUIRE(space.size() == 3);
	REQUIRE(space.contains("one"));
	REQUIRE(space.contains("two"));
	REQUIRE(space.contains("three"));

	REQUIRE(space.find("one") != space.end());
	REQUIRE(space.find("two") != space.end());
	REQUIRE(space.find("three") != space.end());

	auto iter = space.begin();
	REQUIRE(iter != space.end());
	REQUIRE(iter->name == "one");
	REQUIRE(iter->value.isBool());

	++iter;
	REQUIRE(iter != space.end());
	REQUIRE(iter->name == "two");
	REQUIRE(iter->value.isReal());

	++iter;
	REQUIRE(iter != space.end());
	REQUIRE(iter->name == "three");
	REQUIRE(iter->value.isInt());

	++iter;
	REQUIRE(iter == space.end());

	REQUIRE(space.erase("one"));
	REQUIRE_FALSE(space.erase("four"));

	REQUIRE(space.size() == 2);
	REQUIRE_FALSE(space.contains("one"));
}