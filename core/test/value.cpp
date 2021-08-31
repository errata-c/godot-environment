#ifdef _MSC_BUILD
#pragma warning(disable: 26812)
#endif

#include <catch.hpp>
#include <gdev/Value.hpp>

using Value = gdev::Value;
using VType = gdev::ValueType;

TEST_CASE("Value instantiation", "[value]") {
	Value binary = Value::MakeBool(false);
	Value real = Value::MakeReal(0.0);
	Value cat = Value::MakeInt(0);

	Value vecBinary = Value::MakeBool(false, 10);
	Value vecReal = Value::MakeReal(0.0, 10);
	Value vecCat = Value::MakeInt(0, 10);

	REQUIRE(binary.isBool());
	REQUIRE(real.isReal());
	REQUIRE(cat.isInt());

	REQUIRE(binary.asBool() == false);
	REQUIRE(real.asReal() == 0.0);
	REQUIRE(cat.asInt() == 0);

	REQUIRE(binary == false);

	REQUIRE_THROWS(binary.asReal());
	REQUIRE_THROWS(binary.asInt());

	REQUIRE_THROWS(real.asBool());
	REQUIRE_THROWS(real.asInt());

	REQUIRE_THROWS(cat.asReal());
	REQUIRE_THROWS(cat.asBool());

	REQUIRE(vecBinary.size() == 10);
	REQUIRE(vecReal.size() == 10);
	REQUIRE(vecCat.size() == 10);

	REQUIRE(binary.size() == 1);
	REQUIRE(real.size() == 1);
	REQUIRE(cat.size() == 1);

	REQUIRE(vecBinary.begin() != vecBinary.end());
	REQUIRE(vecReal.begin() != vecReal.end());
	REQUIRE(vecCat.begin() != vecCat.end());

	REQUIRE(vecBinary.end() - vecBinary.begin() == vecBinary.size());
	REQUIRE(vecReal.end() - vecReal.begin() == vecReal.size());
	REQUIRE(vecCat.end() - vecCat.begin() == vecCat.size());

	REQUIRE_NOTHROW(vecBinary[1].type() == VType::Bool);
	REQUIRE_NOTHROW(vecReal[1].type() == VType::Real);
	REQUIRE_NOTHROW(vecCat[1].type() == VType::Int);

	REQUIRE_THROWS(binary[1]);
	REQUIRE_THROWS(real[1]);
	REQUIRE_THROWS(cat[1]);

	REQUIRE_THROWS(vecBinary[11]);
	REQUIRE_THROWS(vecReal[11]);
	REQUIRE_THROWS(vecCat[11]);
}

TEST_CASE("Value move, copy, equality", "[value]") {
	Value binary = Value::MakeBool(false);
	Value real = Value::MakeReal(0.0);
	Value cat = Value::MakeInt(0);

	Value vecBinary = Value::MakeBool(false, 10);
	Value vecReal = Value::MakeReal(0.0, 10);
	Value vecCat = Value::MakeInt(0, 10);

	Value binaryCopy = binary;
	Value realCopy = real;
	Value catCopy = cat;

	REQUIRE(binaryCopy.isBool());
	REQUIRE(realCopy.isReal());
	REQUIRE(catCopy.isInt());

	REQUIRE(binary.asBool() == binaryCopy.asBool());
	REQUIRE(real.asReal() == realCopy.asReal());
	REQUIRE(cat.asInt() == catCopy.asInt());

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