#ifdef _MSC_BUILD
#pragma warning(disable: 26812)
#endif

#include <catch.hpp>
#include <gdev/Value.hpp>
#include <gdev/ValueDef.hpp>
#include <gdev/Space.hpp>
#include <gdev/SpaceDef.hpp>


TEST_CASE("serialization") {
	gdev::Value binary = gdev::Value::MakeBool(true);
	gdev::Value real = gdev::Value::MakeReal(0.5);
	gdev::Value cat = gdev::Value::MakeInt(2);

	gdev::Value vecBinary = gdev::Value::MakeBool(true, 10);
	gdev::Value vecReal = gdev::Value::MakeReal(0.5, 10);
	gdev::Value vecCat = gdev::Value::MakeInt(2, 10);


}