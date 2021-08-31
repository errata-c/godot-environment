#include <gdev/Value.hpp>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <limits>

namespace gdev {
	using iterator = Value::iterator;
	using const_iterator = Value::const_iterator;

	Value Value::MakeBinary(bool val, int x, int y, int z, int w) {
		if ((x < 1) || (y < 1) || (z < 1) || (w < 1)) {
			throw std::logic_error("Invalid dimension for Value type!");
		}
		
		return Value(Type::Bool, dim_t{x, y, z, w}, val);
	}
	Value Value::MakeReal(double val, int x, int y, int z, int w) {
		if ((x < 1) || (y < 1) || (z < 1) || (w < 1)) {
			throw std::logic_error("Invalid dimension for Value type!");
		}

		return Value(Type::Real, dim_t{x, y, z, w}, val);
	}
	Value Value::MakeCategorical(int val, int x, int y, int z, int w) {
		if ((x < 1) || (y < 1) || (z < 1) || (w < 1)) {
			throw std::logic_error("Invalid dimension for Value type!");
		}

		return Value(Type::Int, dim_t{x, y, z, w}, val);
	}

	Value::Value(bool val) noexcept
		: Value(Type::Bool, {1,1,1,1}, val)
	{}
	Value::Value(int val) noexcept
		: Value(Type::Int, { 1,1,1,1 }, val)
	{}
	Value::Value(double val) noexcept 
		: Value(Type::Real, { 1,1,1,1 }, val)
	{}

	Value::Value(const Value& other)
		: mtype(other.type())
		, mdims(other.dims())
		, data(nullptr)
	{
		std::memcpy(buffer, other.buffer, sizeof(other.buffer));
		if (other.size() == 1) {
			data = &binary;
		}
		else {
			switch (other.type()) {
			case ValueType::Bool:
				data = calloc(size(), sizeof(bool));
				std::copy((const bool*)other.begin(), (const bool *)other.end(), (bool*)begin());
				break;
			case ValueType::Int:
				data = calloc(size(), sizeof(int));
				std::copy((const int*)other.begin(), (const int*)other.end(), (int*)begin());
				break;
			case ValueType::Real:
				data = calloc(size(), sizeof(double));
				std::copy((const double*)other.begin(), (const double*)other.end(), (double*)begin());
				break;
			}
		}
	}
	Value::Value(Value&& other) noexcept
		: mdims(other.mdims)
		, mtype(other.type())
		, data(other.data)
	{
		std::memcpy(buffer, other.buffer, sizeof(other.buffer));
		if (size() == 1) {
			data = &binary;
		}
		else {
			other.data = nullptr;
		}
	}
	Value& Value::operator=(const Value& other) {
		Value::~Value();
		new (this) Value(other);
		return *this;
	}
	Value& Value::operator=(Value&& other) noexcept {
		Value::~Value();
		new (this) Value(std::move(other));
		return *this;
	}
	Value::~Value() {
		if (size() > 1) {
			free(data);
			data = nullptr;
		}
	}

	Value::Value() noexcept
		: mtype(Type::Real)
		, mdims{1,1,1,1}
		, data(&real)
	{
		real = 0.0;
	}
	Value::Value(Type _type, dim_t _dims, bool val)
		: mtype(_type)
		, mdims{ _dims }
		, data(&binary)
	{
		std::memset(buffer, 0, sizeof(buffer));
		binary = val;
		
		if (size() > 1) {
			std::fill((bool *)begin(), (bool *)end(), val);
		}
	}
	Value::Value(Type _type, dim_t _dims, int val)
		: mtype(_type)
		, mdims{ _dims }
		, data(&category)
	{
		std::memset(buffer, 0, sizeof(buffer));
		category = val;

		if (size() > 1) {
			std::fill((int*)begin(), (int*)end(), val);
		}
	}
	Value::Value(Type _type, dim_t _dims, double val)
		: mtype(_type)
		, mdims{ _dims }
		, data(&real)
	{
		real = val;

		if (size() > 1) {
			std::fill((double*)begin(), (double*)end(), val);
		}
	}


	bool& Value::asBool() {
		if (!isBool()) {
			throw std::logic_error("Attempt to cast non-binary Value to binary!");
		}
		return *(bool*)data;
	}
	const bool& Value::asBool() const {
		if (!isBool()) {
			throw std::logic_error("Attempt to cast non-binary Value to binary!");
		}
		return *(const bool*)data;
	}

	double& Value::asReal() {
		if (!isReal()) {
			throw std::logic_error("Attempt to cast non-real Value to real!");
		}
		return *(double*)data;
	}
	const double& Value::asReal() const {
		if (!isReal()) {
			throw std::logic_error("Attempt to cast non-real Value to real!");
		}
		return *(const double*)data;
	}

	int& Value::asInt() {
		if (!isInt()) {
			throw std::logic_error("Attempt to cast non-categorical Value to category!");
		}
		return *(int*)data;
	}
	const int& Value::asInt() const {
		if (!isInt()) {
			throw std::logic_error("Attempt to cast non-categorical Value to category!");
		}
		return *(const int*)data;
	}


	bool Value::isBool() const noexcept {
		return type() == Type::Bool;
	}
	bool Value::isReal() const noexcept {
		return type() == Type::Real;
	}
	bool Value::isInt() const noexcept {
		return type() == Type::Int;
	}

	std::size_t Value::size() const noexcept {
		return std::size_t(mdims[0]) * mdims[1] * mdims[2] * mdims[3];
	}
	ValueType Value::type() const noexcept {
		return mtype;
	}
	

	bool Value::equal(const Value& other) const noexcept {
		if (type() != other.type()) {
			return false;
		}
		if (dims() != other.dims()) {
			return false;
		}
		
		switch (type()) {
		case ValueType::Bool: {
			std::size_t count = size();
			const bool * ours = (const bool *)data;
			const bool * theirs = (const bool *)other.data;
			const bool * last = ours + count;
			for (; ours != last; ++ours, ++theirs) {
				if (*ours != *theirs) {
					return false;
				}
			}
			return true;
		}
		case ValueType::Real: {
			std::size_t count = size();
			const double* ours = (const double*)data;
			const double* theirs = (const double*)other.data;
			const double* last = ours + count;
			for (; ours != last; ++ours, ++theirs) {
				if (std::abs(*ours - *theirs) >= 1e-5) {
					return false;
				}
			}
			return true;
		}
		case ValueType::Int: {
			std::size_t count = size();
			const int* ours = (const int*)data;
			const int* theirs = (const int*)other.data;
			const int* last = ours + count;
			for (; ours != last; ++ours, ++theirs) {
				if (*ours != *theirs) {
					return false;
				}
			}
			return true;
		}
		default:
			return false;
		}
	}
	bool Value::nequal(const Value& other) const noexcept {
		return !equal(other);
	}

	ValueRef Value::at(int i) {
		if (i >= size()) {
			throw std::out_of_range("Access out of bounds element in Value Vector!");
		}
		
		switch (type()) {
		case ValueType::Bool:
			return ValueRef(((bool *)data)[i]);
		case ValueType::Int:
			return ValueRef(((int*)data)[i]);
		case ValueType::Real:
			return ValueRef(((double*)data)[i]);
		}
	}
	const ValueRef Value::at(int i) const {
		if (i >= size()) {
			throw std::out_of_range("Access out of bounds element in Value Vector!");
		}

		switch (type()) {
		case ValueType::Bool:
			return ValueRef(((bool*)data)[i]);
		case ValueType::Int:
			return ValueRef(((int*)data)[i]);
		case ValueType::Real:
			return ValueRef(((double*)data)[i]);
		}
	}

	ValueRef Value::operator[](int i){
		return at(i);
	}
	const ValueRef Value::operator[](int i) const {
		return at(i);
	}


	iterator Value::begin() {
		return iterator(data, type());
	}
	iterator Value::end() {
		return begin() + size();
	}

	const_iterator Value::begin() const {
		return const_iterator(data, type());
	}
	const_iterator Value::end() const {
		return begin() + size();
	}

	bool Value::operator==(const gdev::Value& rh) const noexcept {
		return equal(rh);
	}
	bool Value::operator!=(const gdev::Value& rh) const noexcept {
		return nequal(rh);
	}

	bool Value::operator==(bool rh) const noexcept {
		if (isBool() && size() == 1) {
			return binary == rh;
		}
		else {
			return false;
		}
	}
	bool Value::operator!=(bool rh) const noexcept {
		if (isBool() && size() == 1) {
			return binary != rh;
		}
		else {
			return true;
		}
	}

	bool Value::operator==(int rh) const noexcept {
		if (isInt() && size() == 1) {
			return category == rh;
		}
		else {
			return false;
		}
	}
	bool Value::operator!=(int rh) const noexcept {
		if (isInt() && size() == 1) {
			return category != rh;
		}
		else {
			return true;
		}
	}

	bool Value::operator==(double rh) const noexcept {
		if (isReal() && size() == 1) {
			return real == rh;
		}
		else {
			return false;
		}
	}
	bool Value::operator!=(double rh) const noexcept {
		if (isReal() && size() == 1) {
			return real != rh;
		}
		else {
			return true;
		}
	}

	std::size_t Value::elementSize() const noexcept {
		switch (type()) {
		case ValueType::Bool:
			return sizeof(bool);
		case ValueType::Int:
			return sizeof(int);
		case ValueType::Real:
			return sizeof(double);
		}
	}
	dim_t Value::dims() const noexcept {
		return mdims;
	}
	int Value::dim(int index) const noexcept {
		return mdims[index];
	}
	int Value::width() const noexcept {
		return mdims[0];
	}
	int Value::height() const noexcept {
		return mdims[1];
	}
	int Value::numDims() const noexcept {
		int count = 0;
		for (int d : mdims) {
			if (d > 1) {
				++count;
			}
		}
		return std::max(1, count);
	}
}


bool operator==(bool lh, const gdev::Value& rh) noexcept {
	return rh == lh;
}
bool operator!=(bool lh, const gdev::Value& rh) noexcept {
	return rh != lh;
}


bool operator==(int lh, const gdev::Value& rh) noexcept {
	return rh == lh;
}
bool operator!=(int lh, const gdev::Value& rh) noexcept {
	return rh != lh;
}


bool operator==(double lh, const gdev::Value& rh) noexcept {
	return rh == lh;
}
bool operator!=(double lh, const gdev::Value& rh) noexcept {
	return rh != lh;
}