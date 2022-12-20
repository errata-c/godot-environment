#include <gdev/Value.hpp>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <limits>

namespace gdev {
	using iterator = Value::iterator;
	using const_iterator = Value::const_iterator;


	iterator::iterator() noexcept
		: data(nullptr)
		, step(0)
		, type(ValueType::b8)
	{}
	iterator::iterator(void* _data, ValueType _type) noexcept
		: data((char*)_data)
		, type(_type)
		, step(SizeOf(_type))
	{}

	bool iterator::operator==(const iterator& other) const noexcept {
		return data == other.data;
	}
	bool iterator::operator!=(const iterator& other) const noexcept {
		return data != other.data;
	}
	bool iterator::operator<(const iterator& other) const noexcept {
		return data < other.data;
	}
	bool iterator::operator<=(const iterator& other) const noexcept {
		return data <= other.data;
	}
	bool iterator::operator>(const iterator& other) const noexcept {
		return data > other.data;
	}
	bool iterator::operator>=(const iterator& other) const noexcept {
		return data >= other.data;
	}

	iterator::reference iterator::operator*() noexcept {
		return ValueRef(data, type);
	}
	iterator::pointer iterator::operator->() noexcept {
		return ValueRef(data, type);
	}
	iterator::reference iterator::operator[](difference_type offset) noexcept {
		return *((*this) + offset);
	}

	iterator& iterator::operator++() noexcept {
		data += step;
		return *this;
	}
	iterator iterator::operator++(int) noexcept {
		iterator copy = *this;
		++*this;
		return copy;
	}

	iterator& iterator::operator--() noexcept {
		data -= step;
		return *this;
	}
	iterator iterator::operator--(int) noexcept {
		iterator copy = *this;
		--* this;
		return copy;
	}

	iterator& iterator::operator+=(difference_type offset) noexcept {
		data += offset * step;
		return *this;
	}
	iterator& iterator::operator-=(difference_type offset) noexcept {
		data -= offset * step;
		return *this;
	}

	iterator iterator::operator+(difference_type offset) const noexcept {
		iterator copy = *this;
		copy += offset;
		return copy;
	}
	iterator iterator::operator-(difference_type offset) const noexcept {
		iterator copy = *this;
		copy -= offset;
		return copy;
	}

	iterator::difference_type iterator::operator-(iterator offset) const noexcept {
		difference_type diff = data - offset.data;
		return diff / step;
	}




	const_iterator::const_iterator() noexcept
		: data(nullptr)
		, step(0)
		, type(ValueType::b8)
	{}
	const_iterator::const_iterator(const void* _data, ValueType _type) noexcept
		: data((char*)_data)
		, type(_type)
		, step(SizeOf(_type))
	{}
	const_iterator::const_iterator(iterator other) noexcept
		: data(other.data)
		, step(other.step)
		, type(other.type)
	{}

	bool const_iterator::operator==(const const_iterator& other) const noexcept {
		return data == other.data;
	}
	bool const_iterator::operator!=(const const_iterator& other) const noexcept {
		return data != other.data;
	}
	bool const_iterator::operator<(const const_iterator& other) const noexcept {
		return data < other.data;
	}
	bool const_iterator::operator<=(const const_iterator& other) const noexcept {
		return data <= other.data;
	}
	bool const_iterator::operator>(const const_iterator& other) const noexcept {
		return data > other.data;
	}
	bool const_iterator::operator>=(const const_iterator& other) const noexcept {
		return data >= other.data;
	}

	const_iterator::reference const_iterator::operator*() noexcept {
		return ValueRef((void*)data, type);
	}
	const_iterator::pointer const_iterator::operator->() noexcept {
		return ValueRef((void*)data, type);
	}
	const_iterator::reference const_iterator::operator[](difference_type offset) noexcept {
		return *((*this) + offset);
	}

	const_iterator& const_iterator::operator++() noexcept {
		data += step;
		return *this;
	}
	const_iterator const_iterator::operator++(int) noexcept {
		const_iterator copy = *this;
		++* this;
		return copy;
	}

	const_iterator& const_iterator::operator--() noexcept {
		data -= step;
		return *this;
	}
	const_iterator const_iterator::operator--(int) noexcept {
		const_iterator copy = *this;
		--* this;
		return copy;
	}

	const_iterator& const_iterator::operator+=(difference_type offset) noexcept {
		data += offset * step;
		return *this;
	}
	const_iterator& const_iterator::operator-=(difference_type offset) noexcept {
		data -= offset * step;
		return *this;
	}

	const_iterator const_iterator::operator+(difference_type offset) const noexcept {
		const_iterator copy = *this;
		copy += offset;
		return copy;
	}
	const_iterator const_iterator::operator-(difference_type offset) const noexcept {
		const_iterator copy = *this;
		copy -= offset;
		return copy;
	}

	const_iterator::difference_type const_iterator::operator-(const_iterator offset) const noexcept {
		difference_type diff = offset.data - data;
		return diff / step;
	}
}