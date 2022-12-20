#include <catch2/catch_all.hpp>
#include <gdev/Value.hpp>

using namespace gdev;
using VType = gdev::ValueType;

TEST_CASE("Value move construct", "[value]") {
	Value val0(0, VType::i32);

	Value val1(std::move(val0));

	//REQUIRE(val0.elements() == 0);
	REQUIRE(val1.elements() == 1);
	REQUIRE(val1.type() == VType::i32);
	REQUIRE(val1[0] == 0);
}

TEST_CASE("Value move assign", "[value]") {
	Value val0(0, VType::i32);
	Value val1(10, VType::f32);

	val1 = std::move(val0);


}

/*
TEST_CASE("Value move, copy, equality", "[value]") {
	Value binary(false);
	Value real(0.0);
	Value cat(0);

	Value vecBinary(false, dim_t{ 10, 1, 1, 1 });
	Value vecReal(0.0, dim_t{ 10, 1, 1, 1 });
	Value vecCat(0, dim_t{ 10, 1, 1, 1 });

	Value binaryCopy = binary;
	Value realCopy = real;
	Value catCopy = cat;

	REQUIRE(binaryCopy.type() == ValueType::b8);
	REQUIRE(realCopy.type() == ValueType::f64);
	REQUIRE(catCopy.type() == ValueType::i32);

	REQUIRE(binary == binaryCopy);
	REQUIRE(real == realCopy);
	REQUIRE(cat == catCopy);

	REQUIRE(binary != real);
	REQUIRE(binary != cat);

	REQUIRE(real != binary);
	REQUIRE(real != cat);

	REQUIRE(cat != binary);
	REQUIRE(cat != real);

	REQUIRE(vecBinary != binary);
	REQUIRE(vecReal != real);
	REQUIRE(vecCat != cat);

	Value vecBinaryCopy = vecBinary;
	Value vecRealCopy = vecReal;
	Value vecCatCopy = vecCat;

	REQUIRE(vecBinaryCopy.type() == vecBinary.type());
	REQUIRE(vecRealCopy.type() == vecReal.type());
	REQUIRE(vecCatCopy.type() == vecCat.type());

	REQUIRE(vecBinaryCopy == vecBinary);
	REQUIRE(vecRealCopy == vecReal);
	REQUIRE(vecCatCopy == vecCat);

	REQUIRE(vecBinaryCopy.begin() != vecBinary.begin());
	REQUIRE(vecRealCopy.begin() != vecReal.begin());
	REQUIRE(vecCatCopy.begin() != vecCat.begin());
}
*/