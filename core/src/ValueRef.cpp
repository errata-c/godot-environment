#include <gdev/ValueRef.hpp>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <limits>

namespace gdev {
	ValueRef::ValueRef(const ValueRef& val) noexcept
		: mtype(val.type())
		, mdata(val.mdata)
	{}
	ValueRef::ValueRef(void* val, ValueType _type) noexcept
		: mtype(_type)
		, mdata(val)
	{}

	ValueRef& ValueRef::operator=(const bool& val) {
		// No other type is implicitly convertible to bool
		if (type() != ValueType::b8) {
			throw std::logic_error("");
		}
		_as<bool>() = val;
		return *this;
	}
	ValueRef& ValueRef::operator=(const int8_t& val) {
		return operator=(int32_t(val));
	}
	ValueRef& ValueRef::operator=(const int16_t& val) {
		return operator=(int32_t(val));
	}
	ValueRef& ValueRef::operator=(const int32_t& val) {
		switch (type()) {
		case ValueType::i8:
			_as<int8_t>() = val;
			break;
		case ValueType::i16:
			_as<int16_t>() = val;
			break;
		case ValueType::i32:
			_as<int32_t>() = val;
			break;
		case ValueType::i64:
			_as<int64_t>() = val;
			break;

		case ValueType::f32:
			_as<float>() = val;
			break;
		case ValueType::f64:
			_as<double>() = val;
			break;
		default:
			throw std::logic_error("");
		}
	}
	ValueRef& ValueRef::operator=(const int64_t& val) {
		switch (type()) {
		case ValueType::i8:
			_as<int8_t>() = val;
			break;
		case ValueType::i16:
			_as<int16_t>() = val;
			break;
		case ValueType::i32:
			_as<int32_t>() = val;
			break;
		case ValueType::i64:
			_as<int64_t>() = val;
			break;

		case ValueType::f32:
			_as<float>() = val;
			break;
		case ValueType::f64:
			_as<double>() = val;
			break;
		default:
			throw std::logic_error("");
		}
		return *this;
	}
	ValueRef& ValueRef::operator=(const float& val) {
		switch (type()) {
		case ValueType::i8:
			_as<int8_t>() = val;
			break;
		case ValueType::i16:
			_as<int16_t>() = val;
			break;
		case ValueType::i32:
			_as<int32_t>() = val;
			break;
		case ValueType::i64:
			_as<int64_t>() = val;
			break;

		case ValueType::f32:
			_as<float>() = val;
			break;
		case ValueType::f64:
			_as<double>() = val;
			break;
		default:
			throw std::logic_error("");
		}
		return *this;
	}
	ValueRef& ValueRef::operator=(const double& val) {
		switch (type()) {
		case ValueType::i8:
			_as<int8_t>() = val;
			break;
		case ValueType::i16:
			_as<int16_t>() = val;
			break;
		case ValueType::i32:
			_as<int32_t>() = val;
			break;
		case ValueType::i64:
			_as<int64_t>() = val;
			break;

		case ValueType::f32:
			_as<float>() = val;
			break;
		case ValueType::f64:
			_as<double>() = val;
			break;
		default:
			throw std::logic_error("");
		}
		return *this;
	}


	bool ValueRef::operator==(const ValueRef& other) const {
		return ConstValueRef(*this) == ConstValueRef(other);
	}
	bool ValueRef::operator!=(const ValueRef& other) const {
		return ConstValueRef(*this) != ConstValueRef(other);
	}
	bool ValueRef::operator==(const ConstValueRef& val) const {
		return ConstValueRef(*this) == val;
	}
	bool ValueRef::operator!=(const ConstValueRef& val) const {
		return ConstValueRef(*this) != val;
	}

	
	
	ValueRef& ValueRef::operator=(const ValueRef& val) {
		if (type() == val.type()) {
			// Exact match
			
		}
		else if (is_integer(type())) {
			
		}
		else if(is_floating_point(type())) {
			
		}
		else {
			throw std::logic_error{ "error" };
		}
	}
	ValueType ValueRef::type() const noexcept {
		return mtype;
	}



	ConstValueRef::ConstValueRef(const ValueRef& val) noexcept
		: mtype(val.type())
		, mdata(val.mdata)
	{}
	ConstValueRef::ConstValueRef(const ConstValueRef& val) noexcept
		: mtype(val.type())
		, mdata(val.mdata)
	{}
	ConstValueRef::ConstValueRef(const void* val, ValueType _type) noexcept
		: mtype(_type)
		, mdata(val)
	{}

	bool ConstValueRef::operator==(const ValueRef& val) const {
		return *this == ConstValueRef(val);
	}
	bool ConstValueRef::operator!=(const ValueRef& val) const {
		return *this != ConstValueRef(val);
	}

	ValueType ConstValueRef::type() const noexcept {
		return mtype;
	}


	bool ConstValueRef::operator==(const bool& val) const {
		if (type() == ValueType::b8) {
			return _as<bool>() == val;
		}
		else {
			throw std::logic_error("");
		}
	}
	bool ConstValueRef::operator==(const int8_t& val) const {
		return operator==(int32_t(val));
	}
	bool ConstValueRef::operator==(const int16_t& val) const {
		return operator==(int32_t(val));
	}
	bool ConstValueRef::operator==(const int32_t& val) const {
		switch (type()) {
		case ValueType::i8:
			return _as<int8_t>() == val;
		case ValueType::i16:
			return _as<int16_t>() == val;
		case ValueType::i32:
			return _as<int32_t>() == val;

		case ValueType::i64:
			return _as<int64_t>() == val;

		case ValueType::f32:
			return _as<float>() == val;

		case ValueType::f64:
			return _as<float>() == val;
		default:
			throw std::logic_error("");
		}
	}
	bool ConstValueRef::operator==(const int64_t& val) const {
		switch (type()) {
		case ValueType::i8:
			return _as<int8_t>() == val;
		case ValueType::i16:
			return _as<int16_t>() == val;
		case ValueType::i32:
			return _as<int32_t>() == val;
		case ValueType::i64:
			return _as<int64_t>() == val;

		case ValueType::f32:
			return _as<float>() == val;
		case ValueType::f64:
			return _as<double>() == val;
		default:
			throw std::logic_error("");
		}
	}
	bool ConstValueRef::operator==(const float& val) const {
		switch (type()) {
		case ValueType::i8:
			return _as<int8_t>() == val;
		case ValueType::i16:
			return _as<int16_t>() == val;
		case ValueType::i32:
			return _as<int32_t>() == val;
		case ValueType::i64:
			return _as<int64_t>() == val;

		case ValueType::f32:
			return _as<float>() == val;
		case ValueType::f64:
			return _as<double>() == val;
		default:
			throw std::logic_error("");
		}
	}
	bool ConstValueRef::operator==(const double& val) const {
		switch (type()) {
		case ValueType::i8:
			return _as<int8_t>() == val;
		case ValueType::i16:
			return _as<int16_t>() == val;
		case ValueType::i32:
			return _as<int32_t>() == val;
		case ValueType::i64:
			return _as<int64_t>() == val;

		case ValueType::f32:
			return _as<float>() == val;
		case ValueType::f64:
			return _as<double>() == val;
		default:
			throw std::logic_error("");
		}
	}

	bool ConstValueRef::operator==(const ConstValueRef& other) const {
		switch (other.type()) {
		default:
			assert(false);
			return false;

		case ValueType::b8:
			return operator==(other._as<bool>());

		case ValueType::i8:
			return operator==(other._as<int8_t>());
		case ValueType::i16:
			return operator==(other._as<int16_t>());
		case ValueType::i32:
			return operator==(other._as<int32_t>());
		case ValueType::i64:
			return operator==(other._as<int64_t>());

		case ValueType::f32:
			return operator==(other._as<float>());
		case ValueType::f64:
			return operator==(other._as<double>());
		}
	}
	bool ConstValueRef::operator!=(const ConstValueRef& val) const {
		return !operator==(val);
	}
}