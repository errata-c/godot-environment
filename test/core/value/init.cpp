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
TEST_CASE("Value f32", "[value]") {
	Value val(0, ValueType::f32);

	REQUIRE(val.type() == ValueType::f32);
	REQUIRE(val.elements() == 1);
	REQUIRE(val.dims()[0] == 1);
	REQUIRE(val.dims()[1] == 1);
	REQUIRE(val.dims()[2] == 1);
	REQUIRE(val.dims()[3] == 1);

	auto ref = val.at(0);
	REQUIRE(ref.type() == ValueType::f32);

	REQUIRE(ref.as<float>() == 0.f);
	REQUIRE(ref == 0.f);
}
TEST_CASE("Value f64", "[value]") {
	Value val(0, ValueType::f64);

	REQUIRE(val.type() == ValueType::f64);
	REQUIRE(val.elements() == 1);
	REQUIRE(val.dims()[0] == 1);
	REQUIRE(val.dims()[1] == 1);
	REQUIRE(val.dims()[2] == 1);
	REQUIRE(val.dims()[3] == 1);

	auto ref = val.at(0);
	REQUIRE(ref.type() == ValueType::f64);

	REQUIRE(ref.as<double>() == 0.0);
	REQUIRE(ref == 0.0);
}
TEST_CASE("Value b8", "[value]") {
	Value val(false, ValueType::b8);

	REQUIRE(val.type() == ValueType::b8);
	REQUIRE(val.elements() == 1);
	REQUIRE(val.dims()[0] == 1);
	REQUIRE(val.dims()[1] == 1);
	REQUIRE(val.dims()[2] == 1);
	REQUIRE(val.dims()[3] == 1);

	auto ref = val.at(0);
	REQUIRE(ref.type() == ValueType::b8);

	REQUIRE(ref.as<bool>() == false);
	REQUIRE(ref == false);
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
