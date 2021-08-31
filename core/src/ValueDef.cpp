#include <gdev/ValueDef.hpp>
#include <cassert>
#include <stdexcept>
#include <algorithm>

namespace gdev {
	ValueDef ValueDef::MakeBinary(dim_t dims) {
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

	ValueDef ValueDef::MakeCategorical(dim_t dims, int low, int high) {
		if ((dims[0] < 1) || (dims[1] < 1) || (dims[2] < 1) || (dims[3] < 1)) {
			throw std::logic_error("Invalid dimensions for ValueDef!");
		}

		return ValueDef(ValueType::Int, dims, low, high);
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
			
			break;
		}
		case ValueType::Int: {

			break;
		}
		case ValueType::Real: {

			break;
		}
		}

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
		double mid = (low + high) * 0.5;
		
		Value ret;
		switch (type()) {
		case ValueType::Bool:
			ret = Value::MakeBinary(false);
			break;
		case ValueType::Int:
			ret = Value::MakeCategorical(int(low));
			break;
		case ValueType::Real:
			ret = Value::MakeReal(low);
			break;
		}

		return ret;
	}
}