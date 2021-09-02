#include <gdev/ValueDef.hpp>
#include <cassert>
#include <stdexcept>
#include <algorithm>

namespace gdev {
	ValueDef ValueDef::MakeBool(dim_t dims) {
		if ((dims[0] < 1) || (dims[1] < 1) || (dims[2] < 1) || (dims[3] < 1)) {
			throw std::logic_error("Invalid dimensions for ValueDef!");
		}

		return ValueDef(ValueType::Bool, dims, 0.0, 1.0);
	}

	ValueDef ValueDef::MakeReal(dim_t dims, double low, double high) {
		if ((dims[0] < 1) || (dims[1] < 1) || (dims[2] < 1) || (dims[3] < 1)) {
			throw std::logic_error("Invalid dimensions for ValueDef!");
		}

		return ValueDef(ValueType::Real, dims, low, high);
	}
	ValueDef ValueDef::MakeReal(dim_t dims, range_t range) {
		return MakeReal(dims, range[0], range[1]);
	}

	ValueDef ValueDef::MakeInt(dim_t dims, int low, int high) {
		if ((dims[0] < 1) || (dims[1] < 1) || (dims[2] < 1) || (dims[3] < 1)) {
			throw std::logic_error("Invalid dimensions for ValueDef!");
		}

		return ValueDef(ValueType::Int, dims, low, high);
	}
	ValueDef ValueDef::MakeInt(dim_t dims, range_t range) {
		return MakeInt(dims, range[0], range[1]);
	}

	ValueDef::ValueDef() noexcept
		: mtype(ValueType::Real)
		, low(0)
		, high(0)
	{
		mdims.fill(1);
	}
	ValueDef::ValueDef(ValueType _type, dim_t _dims, double _low, double _high) noexcept
		: mtype(_type)
		, mdims(_dims)
		, low(_low)
		, high(_high)
	{}

	ValueType ValueDef::type() const noexcept {
		return mtype;
	}

	bool ValueDef::contains(bool val) const noexcept {
		if (type() == ValueType::Bool) {
			return true;
		}
		else {
			return false;
		}
	}
	bool ValueDef::contains(int val) const noexcept {
		if (type() == ValueType::Int) {
			return (double(val) >= low) && (double(val) <= high);
		}
		else {
			return false;
		}
	}
	bool ValueDef::contains(double val) const noexcept {
		if (type() == ValueType::Real) {
			return (val >= low) && (val <= high);
		}
		else {
			return false;
		}
	}
	bool ValueDef::contains(const Value& val) const noexcept {
		if (type () != val.type()) {
			return false;
		}
		if (dims() != val.dims()) {
			return false;
		}
		
		switch (val.type()) {
		case ValueType::Bool: {
			return true;
		}
		case ValueType::Int: {
			auto it = (const int*)val.begin();
			auto last = (const int*)val.end();
			for (; it != last; ++it) {
				if (*it < low || *it > high) {
					return false;
				}
			}
			return true;
		}
		case ValueType::Real: {
			auto it = (const double*)val.begin();
			auto last = (const double*)val.end();
			for (; it != last; ++it) {
				if (*it < low || *it > high) {
					return false;
				}
			}
			return true;
		}}

		return false;
	}

	bool ValueDef::isBool() const noexcept {
		return type() == ValueType::Bool;
	}
	bool ValueDef::isReal() const noexcept {
		return type() == ValueType::Real;
	}
	bool ValueDef::isInt() const noexcept {
		return type() == ValueType::Int;
	}

	double ValueDef::lowerBound() const noexcept {
		return low;
	}
	double ValueDef::upperBound() const noexcept {
		return high;
	}
	range_t ValueDef::bounds() const noexcept {
		return range;
	}

	std::size_t ValueDef::size() const noexcept {
		return std::size_t(mdims[0]) * mdims[1] * mdims[2] * mdims[3];
	}
	const dim_t& ValueDef::dims() const noexcept {
		return mdims;
	}
	int ValueDef::dim(int index) const noexcept {
		return mdims[index];
	}
	
	Value ValueDef::instance() const {
		switch (type()) {
		case ValueType::Bool:
			return Value::MakeBool(false, dims());
		case ValueType::Int:
			return Value::MakeInt(low, dims());
		case ValueType::Real:
			return Value::MakeReal(low, dims());
		}

		return {};
	}
}