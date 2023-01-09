#include <catch2/catch_all.hpp>
#include <gdev/Value.hpp>

using namespace gdev;
using VType = gdev::ValueType;

TEST_CASE("Value array access", "[value]") {
	Value test(dim_t{ 10,1,1,1 }, 0, VType::i32);

	for (int i = 0; i < 10; ++i) {
		test[i] = i;
	}

	for (int i = 0; i < 10; ++i) {
		CAPTURE(i);
		REQUIRE(test[i] == i);
	}
	
	for (int i = 0; i < 10; ++i) {
		CAPTURE(i);
		REQUIRE(test.at(i) == i);
	}

	for (int i = 0; i < 10; ++i) {
		CAPTURE(i);
		REQUIRE(test(i) == i);
	}
}

TEST_CASE("Value 2d access", "[value]") {
	Value test(dim_t{ 10,10,1,1 }, 0, VType::i32);

	for (int i = 0; i < test.elements(); ++i) {
		test[i] = i;
	}

	for (int y = 0; y < 10; ++y) {
		for (int x = 0; x < 10; ++x) {
			CAPTURE(x, y);

			REQUIRE(test(x, y) == test[x + y * 10]);
		}
	}
}