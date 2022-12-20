#ifdef _MSC_BUILD
#pragma warning(disable: 26812)
#endif

#include <catch2/catch_all.hpp>
#include <gdev/Value.hpp>

using namespace gdev;
using VType = gdev::ValueType;

TEST_CASE("Value i8", "[value]") {
	Value val(0, ValueType::i8);

	REQUIRE(val.type() == ValueType::i8);
	REQUIRE(val.elements() == 1);
	REQUIRE(val.dims()[0] == 1);
	REQUIRE(val.dims()[1] == 1);
	REQUIRE(val.dims()[2] == 1);
	REQUIRE(val.dims()[3] == 1);

	auto ref = val.at(0);
	REQUIRE(ref.type() == ValueType::i8);

	REQUIRE(ref.as<int8_t>() == 0);
	REQUIRE(ref == 0);
}
TEST_CASE("Value i16", "[value]") {
	Value val(0, ValueType::i16);

	REQUIRE(val.type() == ValueType::i16);
	REQUIRE(val.elements() == 1);
	REQUIRE(val.dims()[0] == 1);
	REQUIRE(val.dims()[1] == 1);
	REQUIRE(val.dims()[2] == 1);
	REQUIRE(val.dims()[3] == 1);

	auto ref = val.at(0);
	REQUIRE(ref.type() == ValueType::i16);

	REQUIRE(ref.as<int16_t>() == 0);
	REQUIRE(ref == 0);
}
TEST_CASE("Value i32", "[value]") {
	Value val(0, ValueType::i32);

	REQUIRE(val.type() == ValueType::i32);
	REQUIRE(val.elements() == 1);
	REQUIRE(val.dims()[0] == 1);
	REQUIRE(val.dims()[1] == 1);
	REQUIRE(val.dims()[2] == 1);
	REQUIRE(val.dims()[3] == 1);

	auto ref = val.at(0);
	REQUIRE(ref.type() == ValueType::i32);

	REQUIRE(ref.as<int32_t>() == 0);
	REQUIRE(ref == 0);
}
TEST_CASE("Value i64", "[value]") {
	Value val(0, ValueType::i64);

	REQUIRE(val.type() == ValueType::i64);
	REQUIRE(val.elements() == 1);
	REQUIRE(val.dims()[0] == 1);
	REQUIRE(val.dims()[1] == 1);
	REQUIRE(val.dims()[2] == 1);
	REQUIRE(val.dims()[3] == 1);

	auto ref = val.at(0);
	REQUIRE(ref.type() == ValueType::i64);

	REQUIRE(ref.as<int64_t>() == 0);
	REQUIRE(ref == 0);
}



TEST_CASE("Value instantiation", "[value]") {
	Value val0(0, dim_t{ 10,1,1,1 }, ValueType::i32);

	REQUIRE(val0.elements() == 10);
	REQUIRE(val0.begin() != val0.end());

	REQUIRE((val0.end() - val0.begin()) == val0.size());

	{
		auto it = val0.begin();
		auto last = val0.end();

		for (; it != last; ++it) {
			ValueRef ref = *it;
			REQUIRE(ref.type() == VType::i32);
			REQUIRE(ref == 0);
		}
	}
}

TEST_CASE("Value move, copy, equality", "[value]") {
	Value binary(false);
	Value real(0.0);
	Value cat(0);

	Value vecBinary(false, dim_t{10, 1, 1, 1});
	Value vecReal(0.0, dim_t{ 10, 1, 1, 1 });
	Value vecCat (0, dim_t{ 10, 1, 1, 1 });

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