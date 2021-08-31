#ifdef _MSC_BUILD
#pragma warning(disable: 26812)
#endif

#include <catch.hpp>
#include <gdev/Value.hpp>
#include <gdev/ValueDef.hpp>
#include <gdev/Space.hpp>
#include <gdev/SpaceDef.hpp>


TEST_CASE("serialization") {
	gdev::Value binary = gdev::Value::MakeBinary(true);
	gdev::Value real = gdev::Value::MakeReal(0.5);
	gdev::Value cat = gdev::Value::MakeCategorical(2);

	gdev::Value vecBinary = gdev::Value::MakeBinary(true, 10);
	gdev::Value vecReal = gdev::Value::MakeReal(0.5, 10);
	gdev::Value vecCat = gdev::Value::MakeCategorical(2, 10);


}