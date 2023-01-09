#include <gdev/Value.hpp>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <limits>
#include <cassert>
#include <ostream>

namespace gdev {
	using iterator = Value::iterator;
	using const_iterator = Value::const_iterator;
	
	Value::Value() noexcept
		: mtype(ValueType::f32)
		, mdims{0,1,1,1}
	{}
	Value::Value(const Value& other)
		: mtype(other.type())
		, mdims(other.dims())
	{
		allocate();
		std::memcpy(data(), other.data(), other.bytes());
	}
	Value::Value(Value&& other) noexcept
		: mdims(other.mdims)
		, mtype(other.type())
		, mdata(std::move(other.mdata))
	{
		other.mdims = dim_t{0, 1, 1, 1};
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
	Value::~Value() {}

	std::size_t Value::size() const noexcept {
		return std::size_t(mdims[0]) * mdims[1] * mdims[2] * mdims[3];
	}
	std::size_t Value::elements() const noexcept {
		return size();
	}
	std::size_t Value::bytes() const noexcept {
		return size() * SizeOf(type());
	}
	bool Value::empty() const noexcept {
		return bool(mdata);
	}

	ValueType Value::type() const noexcept {
		return mtype;
	}
	

	struct EqualVisit {
		template<typename A, typename B>
		static bool apply(const Value& lh, const Value& rh) {
			if constexpr (std::is_same_v<A, bool> != std::is_same_v<B, bool>) {
				throw std::logic_error("gdev::Value unsafe comparison operation!");
			}
			else {
				size_t count = lh.size();
				const A* ours = (const A*)lh.data();
				const A* last = ours + count;

				const B* theirs = (const B*)rh.data();

				for (; ours != last; ++ours, ++theirs) {
					if (*ours != *theirs) {
						return false;
					}
				}
				return true;
			}
		}
	};
	bool Value::equal(const Value& other) const noexcept {
		if (dims() != other.dims()) {
			return false;
		}

		return visitors::double_visit<EqualVisit>(type(), other.type(), *this, other);
	}
	bool Value::nequal(const Value& other) const noexcept {
		return !equal(other);
	}

	ValueRef Value::at(std::size_t i) {
		if (i < 0 || i >= size()) {
			throw std::out_of_range("Access out of bounds element in Value Vector!");
		}
		
		return ValueRef(data() + SizeOf(type()) * i, type());
	}
	ConstValueRef Value::at(std::size_t i) const {
		if (i < 0 || i >= size()) {
			throw std::out_of_range("Access out of bounds element in Value Vector!");
		}
		return ConstValueRef(data() + SizeOf(type()) * i, type());
	}

	ValueRef Value::at(std::size_t i0, std::size_t i1) {
		return at(i0 + dim(0) * i1);
	}
	ConstValueRef Value::at(std::size_t i0, std::size_t i1) const {
		return at(i0 + dim(0) * i1);
	}

	ValueRef Value::at(std::size_t i0, std::size_t i1, std::size_t i2) {
		return at(i0 + dim(0) * i1 + dim(0) * dim(1) * i2);
	}
	ConstValueRef Value::at(std::size_t i0, std::size_t i1, std::size_t i2) const {
		return at(i0 + dim(0) * i1 + dim(0) * dim(1) * i2);
	}

	ValueRef Value::at(std::size_t i0, std::size_t i1, std::size_t i2, std::size_t i3) {
		return at(i0 + dim(0) * i1 + dim(0) * dim(1) * i2 + dim(0) * dim(1) * dim(2) * i3);
	}
	ConstValueRef Value::at(std::size_t i0, std::size_t i1, std::size_t i2, std::size_t i3) const {
		return at(i0 + dim(0) * i1 + dim(0) * dim(1) * i2 + dim(0) * dim(1) * dim(2) * i3);
	}

	ValueRef Value::operator()(std::size_t i0) {
		return at(i0);
	}
	ValueRef Value::operator()(std::size_t i0, std::size_t i1) {
		return at(i0, i1);
	}
	ValueRef Value::operator()(std::size_t i0, std::size_t i1, std::size_t i2) {
		return at(i0, i1, i2);
	}
	ValueRef Value::operator()(std::size_t i0, std::size_t i1, std::size_t i2, std::size_t i3) {
		return at(i0, i1, i2, i3);
	}

	ConstValueRef Value::operator()(std::size_t i0) const {
		return at(i0);
	}
	ConstValueRef Value::operator()(std::size_t i0, std::size_t i1) const {
		return at(i0, i1);
	}
	ConstValueRef Value::operator()(std::size_t i0, std::size_t i1, std::size_t i2) const {
		return at(i0, i1, i2);
	}
	ConstValueRef Value::operator()(std::size_t i0, std::size_t i1, std::size_t i2, std::size_t i3) const {
		return at(i0, i1, i2, i3);
	}

	ValueRef Value::operator[](std::size_t i){
		return at(i);
	}
	ConstValueRef Value::operator[](std::size_t i) const {
		return at(i);
	}


	iterator Value::begin() {
		return iterator(mdata.get(), type());
	}
	iterator Value::end() {
		return begin() + size();
	}

	const_iterator Value::begin() const {
		return const_iterator(mdata.get(), type());
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


	dim_t Value::dims() const noexcept {
		return mdims;
	}
	std::size_t Value::dim(std::size_t index) const noexcept {
		return mdims[index];
	}
	std::size_t Value::width() const noexcept {
		return mdims[0];
	}
	std::size_t Value::height() const noexcept {
		return mdims[1];
	}
	std::size_t Value::numDims() const noexcept {
		int count = 0;
		for (int d : mdims) {
			if (d > 1) {
				++count;
			}
		}
		return std::max(1, count);
	}

	void Value::moddims(const dim_t & ndims) {
		std::size_t count = 1;
		for (int i = 0; i < ndims.size(); ++i) {
			if (ndims[i] < 1) {
				throw std::logic_error("Attempt to change a dimension to a value less then one!");
			}
			count *= ndims[i];
		}
		if (count != size()) {
			throw std::logic_error("Attempt to moddims of gdev::Value to a set of dimensions with invalid element count!");
		}
		mdims = ndims;
	}


	char* Value::data() noexcept {
		return mdata.get();
	}
	const char* Value::data() const noexcept {
		return mdata.get();
	}

	void Value::allocate() {
		mdata.reset(new char[bytes()]);
	}
}

// Type specific implementation of the print setup
struct _printer {
	template<typename T>
	static void apply(std::ostream& os, const gdev::Value& value) {
			
	}
};
std::ostream& operator<<(std::ostream& os, const gdev::Value& value) {
	gdev::visitors::single_visit<_printer>(value.type(), os, value);

	return os;
}