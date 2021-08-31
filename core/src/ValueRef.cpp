#include <gdev/Value.hpp>
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
	}
	ValueRef& ValueRef::operator=(int val) {
		if (type() == ValueType::Int) {
			*(int*)data = val;
		}
		else {
			throw std::logic_error("ValueRef cannot cast to int!");
		}
	}
	ValueRef& ValueRef::operator=(double val) {
		if (type() == ValueType::Real) {
			*(double*)data = val;
		}
		else {
			throw std::logic_error("ValueRef cannot cast to real!");
		}
	}
	ValueRef& ValueRef::operator=(const ValueRef& val) {
		switch (val.type()) {
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
}