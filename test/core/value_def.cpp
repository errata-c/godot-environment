#ifdef _MSC_BUILD
#pragma warning(disable: 26812)
#endif

#include <catch2/catch_all.hpp>

#include <gdev/ValueDef.hpp>
#include <gdev/Value.hpp>

using namespace gdev;

TEST_CASE("ValueDef initialization", "[value def]") {
	ValueDef binary = ValueDef::MakeBool({1,1,1,1});
	ValueDef cat = ValueDef::MakeInt({1,1,1,1}, 0, 100);
	ValueDef real = ValueDef::MakeReal({ 1,1,1,1 }, 0.0, 1.0);

	REQUIRE(binary.contains(true));
	REQUIRE(binary.contains(false));
	REQUIRE(cat.contains(0));
	REQUIRE(cat.contains(50));
	REQUIRE(real.contains(0.5));
	REQUIRE(real.contains(0.0));

	REQUIRE(!binary.contains(0.0));
	REQUIRE(!cat.contains(0.0));
	REQUIRE(!real.contains(false));

	// Make sure there is type checking

	Value binaryInstance = binary.instance();
	Value catInstance = cat.instance();
	Value realInstance = real.instance();

	// Test types
	REQUIRE(binaryInstance.isBool());
	REQUIRE(catInstance.isInt());
	REQUIRE(realInstance.isReal());

	// Test initial values
	REQUIRE(binaryInstance.asBool() == false);
	REQUIRE(catInstance.asInt() == 0);
	REQUIRE(realInstance.asReal() == 0.0);

	// Test contains method
	REQUIRE(binary.contains(binaryInstance));
	REQUIRE(cat.contains(catInstance));
	REQUIRE(real.contains(realInstance));
}

TEST_CASE("ValueDef vectors", "[value def]") {
	ValueDef binary = ValueDef::MakeBool({ 10,1,1,1 });
	ValueDef cat = ValueDef::MakeInt({ 10,1,1,1 }, 0, 100);
	ValueDef real = ValueDef::MakeReal({ 10,1,1,1 }, 0.0, 1.0);

	REQUIRE(binary.size() == 10);
	REQUIRE(cat.size() == 10);
	REQUIRE(real.size() == 10);
}