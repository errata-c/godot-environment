#include <gdev/ValueRef.hpp>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <limits>

namespace gdev {

	ValueRef::ValueRef(bool& val) noexcept
		: mtype(ValueType::Bool)
		, data(&val)
	{}
	ValueRef::ValueRef(int& val) noexcept
		: mtype(ValueType::Bool)
		, data(&val)
	{}
	ValueRef::ValueRef(double& val) noexcept
		: mtype(ValueType::Bool)
		, data(&val)
	{}
	ValueRef::ValueRef(const ValueRef& val) noexcept
		: mtype(val.type())
		, data(val.data)
	{}
	ValueRef::ValueRef(void* val, ValueType _type) noexcept
		: mtype(_type)
		, data(val)
	{}

	ValueRef::operator bool() {
		if (type() == ValueType::Bool) {
			return (bool*)data;
		}
		else {
			throw std::logic_error("ValueRef cannot be cast to bool!");
		}
	}
	ValueRef::operator int() {
		if (type() == ValueType::Int) {
			return *(int*)data;
		}
		else {
			throw std::logic_error("ValueRef cannot be cast to int!");
		}
	}
	ValueRef::operator double() {
		if (type() == ValueType::Real) {
			return *(double*)data;
		}
		else {
			throw std::logic_error("ValueRef cannot be cast to double!");
		}
	}

	bool ValueRef::operator==(bool val) const noexcept {
		if (type() == ValueType::Bool) {
			return (*(const bool*)data) == val;
		}
		else {
			return false;
		}
	}
	bool ValueRef::operator==(int val) const noexcept {
		if (type() == ValueType::Int) {
			return (*(const int*)data) == val;
		}
		else {
			return false;
		}
	}
	bool ValueRef::operator==(double val) const noexcept {
		if (type() == ValueType::Real) {
			double tmp = *(const double*)data;
			return std::abs(val - tmp) < 1e-6;
		}
		else {
			return false;
		}
	}
	bool ValueRef::operator==(const ValueRef& val) const noexcept {
		switch (val.type()) {
		default:
			return false;
		case ValueType::Bool:
			return operator==(*(const bool*)val.data);
		case ValueType::Int:
			return operator==(*(const int*)val.data);
		case ValueType::Real:
			return operator==(*(const double*)val.data);
		}
	}

	bool ValueRef::operator!=(bool val) const noexcept {
		if (type() == ValueType::Bool) {
			return *(const bool*)data != val;
		}
		else {
			return true;
		}
	}
	bool ValueRef::operator!=(int val) const noexcept {
		if (type() == ValueType::Int) {
			return *(const int*)data != val;
		}
		else {
			return true;
		}
	}
	bool ValueRef::operator!=(double val) const noexcept {
		if (type() == ValueType::Real) {
			double tmp = *(const double*)data;
			return std::abs(tmp - val) >= 1e-6;
		}
		else {
			return true;
		}
	}
	bool ValueRef::operator!=(const ValueRef& val) const noexcept {
		switch (val.type()) {
		default:
			return false;
		case ValueType::Bool:
			return operator!=(*(const bool*)val.data);
		case ValueType::Int:
			return operator!=(*(const int*)val.data);
		case ValueType::Real:
			return operator!=(*(const double*)val.data);
		}
	}

	ValueRef& ValueRef::operator=(bool val) {
		if (type() == ValueType::Bool) {
			*(bool*)data = val;
		}
		else {
			throw std::logic_error("ValueRef cannot cast to bool!");
		}
		return *this;
	}
	ValueRef& ValueRef::operator=(int val) {
		if (type() == ValueType::Int) {
			*(int*)data = val;
		}
		else {
			throw std::logic_error("ValueRef cannot cast to int!");
		}
		return *this;
	}
	ValueRef& ValueRef::operator=(double val) {
		if (type() == ValueType::Real) {
			*(double*)data = val;
		}
		else {
			throw std::logic_error("ValueRef cannot cast to real!");
		}
		return *this;
	}
	ValueRef& ValueRef::operator=(const ValueRef& val) {
		switch (val.type()) {
		default:
		case ValueType::Bool:
			return operator=(*(const bool*)val.data);
		case ValueType::Int:
			return operator=(*(const int*)val.data);
		case ValueType::Real:
			return operator=(*(const double*)val.data);
		}
	}
	ValueType ValueRef::type() const noexcept {
		return mtype;
	}

	bool ValueRef::isBool() const noexcept {
		return type() == ValueType::Bool;
	}
	bool ValueRef::isReal() const noexcept {
		return type() == ValueType::Real;
	}
	bool ValueRef::isInt() const noexcept {
		return type() == ValueType::Int;
	}

	bool& ValueRef::asBool() {
		if (type() == ValueType::Bool) {
			return *(bool*)data;
		}
		else {
			throw std::logic_error("ValueRef cannot cast to real!");
		}
	}
	const bool& ValueRef::asBool() const {
		if (type() == ValueType::Bool) {
			return *(bool*)data;
		}
		else {
			throw std::logic_error("ValueRef cannot cast to real!");
		}
	}

	int& ValueRef::asInt() {
		if (type() == ValueType::Int) {
			return *(int*)data;
		}
		else {
			throw std::logic_error("ValueRef cannot cast to real!");
		}
	}
	const int& ValueRef::asInt() const {
		if (type() == ValueType::Int) {
			return *(const int*)data;
		}
		else {
			throw std::logic_error("ValueRef cannot cast to real!");
		}
	}

	double& ValueRef::asReal() {
		if (type() == ValueType::Real) {
			return *(double*)data;
		}
		else {
			throw std::logic_error("ValueRef cannot cast to real!");
		}
	}
	const double& ValueRef::asReal() const {
		if (type() == ValueType::Real) {
			return *(const double*)data;
		}
		else {
			throw std::logic_error("ValueRef cannot cast to real!");
		}
	}









	ConstValueRef::ConstValueRef(const bool& val) noexcept
		: mtype(ValueType::Bool)
		, data(&val)
	{}
	ConstValueRef::ConstValueRef(const int& val) noexcept
		: mtype(ValueType::Bool)
		, data(&val)
	{}
	ConstValueRef::ConstValueRef(const double& val) noexcept
		: mtype(ValueType::Bool)
		, data(&val)
	{}
	ConstValueRef::ConstValueRef(const ValueRef& val) noexcept
		: mtype(val.type())
		, data(val.data)
	{}
	ConstValueRef::ConstValueRef(const ConstValueRef& val) noexcept
		: mtype(val.type())
		, data(val.data)
	{}
	ConstValueRef::ConstValueRef(const void* val, ValueType _type) noexcept
		: mtype(_type)
		, data(val)
	{}

	ConstValueRef::operator bool() {
		if (type() == ValueType::Bool) {
			return (const bool*)data;
		}
		else {
			throw std::logic_error("ValueRef cannot be cast to bool!");
		}
	}
	ConstValueRef::operator int() {
		if (type() == ValueType::Int) {
			return *(const int*)data;
		}
		else {
			throw std::logic_error("ValueRef cannot be cast to int!");
		}
	}
	ConstValueRef::operator double() {
		if (type() == ValueType::Real) {
			return *(const double*)data;
		}
		else {
			throw std::logic_error("ValueRef cannot be cast to double!");
		}
	}

	bool ConstValueRef::operator==(bool val) const noexcept {
		if (type() == ValueType::Bool) {
			return (*(const bool*)data) == val;
		}
		else {
			return false;
		}
	}
	bool ConstValueRef::operator==(int val) const noexcept {
		if (type() == ValueType::Int) {
			return (*(const int*)data) == val;
		}
		else {
			return false;
		}
	}
	bool ConstValueRef::operator==(double val) const noexcept {
		if (type() == ValueType::Real) {
			double tmp = *(const double*)data;
			return std::abs(val - tmp) < 1e-6;
		}
		else {
			return false;
		}
	}
	bool ConstValueRef::operator==(const ValueRef& val) const noexcept {
		switch (val.type()) {
		default:
			return false;
		case ValueType::Bool:
			return operator==(*(const bool*)val.data);
		case ValueType::Int:
			return operator==(*(const int*)val.data);
		case ValueType::Real:
			return operator==(*(const double*)val.data);
		}
	}

	bool ConstValueRef::operator!=(bool val) const noexcept {
		if (type() == ValueType::Bool) {
			return *(const bool*)data != val;
		}
		else {
			return true;
		}
	}
	bool ConstValueRef::operator!=(int val) const noexcept {
		if (type() == ValueType::Int) {
			return *(const int*)data != val;
		}
		else {
			return true;
		}
	}
	bool ConstValueRef::operator!=(double val) const noexcept {
		if (type() == ValueType::Real) {
			double tmp = *(const double*)data;
			return std::abs(tmp - val) >= 1e-6;
		}
		else {
			return true;
		}
	}
	bool ConstValueRef::operator!=(const ValueRef& val) const noexcept {
		switch (val.type()) {
		default:
			return false;
		case ValueType::Bool:
			return operator!=(*(const bool*)val.data);
		case ValueType::Int:
			return operator!=(*(const int*)val.data);
		case ValueType::Real:
			return operator!=(*(const double*)val.data);
		}
	}

	ValueType ConstValueRef::type() const noexcept {
		return mtype;
	}

	bool ConstValueRef::isBool() const noexcept {
		return type() == ValueType::Bool;
	}
	bool ConstValueRef::isReal() const noexcept {
		return type() == ValueType::Real;
	}
	bool ConstValueRef::isInt() const noexcept {
		return type() == ValueType::Int;
	}

	const bool& ConstValueRef::asBool() const {
		if (type() == ValueType::Bool) {
			return *(bool*)data;
		}
		else {
			throw std::logic_error("ValueRef cannot cast to real!");
		}
	}

	const int& ConstValueRef::asInt() const {
		if (type() == ValueType::Int) {
			return *(const int*)data;
		}
		else {
			throw std::logic_error("ValueRef cannot cast to real!");
		}
	}

	const double& ConstValueRef::asReal() const {
		if (type() == ValueType::Real) {
			return *(const double*)data;
		}
		else {
			throw std::logic_error("ValueRef cannot cast to real!");
		}
	}
}