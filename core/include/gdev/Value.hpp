#pragma once
#include <gdev/ValueType.hpp>
#include <gdev/ValueRef.hpp>
#include <gdev/core/Range.hpp>

#include <cinttypes>
#include <cstddef>
#include <vector>
#include <array>
#include <memory>
#include <initializer_list>
#include <type_traits>
#include <iterator>
#include <iosfwd>

namespace gdev {
	class Value {
	public:
		struct iterator;
		struct const_iterator;

		using Type = ValueType;

		Value() noexcept;
		Value(const Value & other);
		Value& operator=(const Value& other);
		Value(Value && other) noexcept;
		Value& operator=(Value && other) noexcept;
		~Value();

private:
		// Uninitialized constructor
		Value(dim_t dims, ValueType id)
			: mtype(id)
			, mdims{ dims }
		{
			allocate();
		}
public:
		static Value Uninitialized(dim_t dims, ValueType id = ValueType::f32) {
			return Value(dims, id);
		}

		// Single value constructor
		template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
		Value(const T& val, ValueType id = ValueTypeTraits<T>::id)
			: Value(dim_t{1,1,1,1}, val, id)
		{}

		// Single value dimensioned constructor
		template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
		Value(dim_t dims, const T& val, ValueType id = ValueTypeTraits<T>::id)
			: mtype(id)
			, mdims{ dims }
		{
			allocate();
			fill(val);
		}

		// flat initializer list constructor
		template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
		Value(std::initializer_list<T> init, ValueType id = ValueTypeTraits<T>::id)
			: Value(dim_t{init.size(), 1, 1, 1}, init.begin(), init.end(), id)
		{}

		// Dimensioned initializer list constructor
		template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
		Value(dim_t dims, std::initializer_list<T> init, ValueType id = ValueTypeTraits<T>::id)
			: mtype(id)
			, mdims(dims)
		{
			assert(elements() == init.size());

			allocate();
			visitors::single_visit<AssignFunc>(type(), data(), data() + bytes(), init.begin(), init.end());
		}

		template<typename InputIt, typename = typename std::iterator_traits<InputIt>::iterator_category>
		Value(dim_t dims, InputIt first, InputIt last, ValueType id = ValueTypeTraits<typename std::iterator_traits<InputIt>::value_type>::id)
			: mtype(id)
			, mdims{dims}
		{
			assert(elements() == init.size());

			allocate();
			visitors::single_visit<AssignFunc>(type(), data(), data() + bytes(), first, last);
		}

		template<typename It, typename = typename std::iterator_traits<It>::iterator_category>
		Value(It first, It last, ValueType id = ValueTypeTraits<typename std::iterator_traits<It>::value_type>::id)
		{
			auto dist = std::distance(first, last);
			assert(dist > 0);
			assign(dim_t{static_cast<std::size_t>(dist), 1, 1, 1}, first, last, id);
		}

		template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
		void fill(const T& val) {
			visitors::single_visit<FillFunc>(type(), data(), data() + bytes(), val);
		}

		template<typename T>
		void assign(dim_t dims, const T& value, ValueType id = ValueTypeTraits<T>::id) {
			mdims = dims;
			mtype = id;
			allocate();
			fill(value);
		}

		// Specific implementation for input iterators, no choice but to copy into a temporary buffer because no multi-pass
		template<typename InputIt, std::enable_if_t<std::is_same_v<typename std::iterator_traits<InputIt>::iterator_category, std::input_iterator_tag>, int> = 0>
		void assign(InputIt first, InputIt last, ValueType id = ValueTypeTraits<typename std::iterator_traits<InputIt>::value_type>::id) {
			std::vector<typename std::iterator_traits<InputIt>::value_type> tmp(first, last);
			assign(dim_t{ tmp.size(), 1, 1, 1 }, tmp.begin(), tmp.end(), id);
		}
		// Implementation for all iterators that are NOT input iterators
		template<typename Iter, std::enable_if_t<!std::is_same_v<typename std::iterator_traits<Iter>::iterator_category, std::input_iterator_tag>, int> = 0>
		void assign(Iter first, Iter last, ValueType id = ValueTypeTraits<typename std::iterator_traits<Iter>::value_type>::id) {
			assign(dim_t{std::distance(first, last), 1, 1, 1}, first, last, id);
		}
		// Implementation available for all iterator types
		template<typename InputIt, typename = typename std::iterator_traits<InputIt>::iterator_category>
		void assign(dim_t dims, InputIt first, InputIt last, ValueType id = ValueTypeTraits<typename std::iterator_traits<InputIt>::value_type>::id) {
			mdims = dims;
			mtype = id;
			allocate();
			visitors::single_visit<AssignFunc>(type(), data(), data() + bytes(), first, last);
		}
		template<typename T>
		void assign(std::initializer_list<T> ilist, ValueType id = ValueTypeTraits<T>::id) {
			assign(dim_t{ilist.size(), 1, 1, 1}, ilist.begin(), ilist.end(), id);
		}
		template<typename T>
		void assign(dim_t dims, std::initializer_list<T> ilist, ValueType id = ValueTypeTraits<T>::id) {
			assign(dims, ilist.begin(), ilist.end(), id);
		}

		std::size_t size() const noexcept;
		std::size_t elements() const noexcept;
		std::size_t bytes() const noexcept;

		bool empty() const noexcept;

		Type type() const noexcept;

		ValueRef at(std::size_t i0);
		ValueRef at(std::size_t i0, std::size_t i1);
		ValueRef at(std::size_t i0, std::size_t i1, std::size_t i2);
		ValueRef at(std::size_t i0, std::size_t i1, std::size_t i2, std::size_t i3);

		ConstValueRef at(std::size_t i0) const;
		ConstValueRef at(std::size_t i0, std::size_t i1) const;
		ConstValueRef at(std::size_t i0, std::size_t i1, std::size_t i2) const;
		ConstValueRef at(std::size_t i0, std::size_t i1, std::size_t i2, std::size_t i3) const;

		ValueRef operator[](std::size_t index);
		ConstValueRef operator[](std::size_t index) const;

		ValueRef operator()(std::size_t i0);
		ValueRef operator()(std::size_t i0, std::size_t i1);
		ValueRef operator()(std::size_t i0, std::size_t i1, std::size_t i2);
		ValueRef operator()(std::size_t i0, std::size_t i1, std::size_t i2, std::size_t i3);

		ConstValueRef operator()(std::size_t i0) const;
		ConstValueRef operator()(std::size_t i0, std::size_t i1) const;
		ConstValueRef operator()(std::size_t i0, std::size_t i1, std::size_t i2) const;
		ConstValueRef operator()(std::size_t i0, std::size_t i1, std::size_t i2, std::size_t i3) const;

		bool equal(const Value & other) const noexcept;
		bool nequal(const Value & other) const noexcept;

		bool operator==(const Value& rh) const noexcept;
		bool operator!=(const Value& rh) const noexcept;

		iterator begin();
		iterator end();

		const_iterator begin() const;
		const_iterator end() const;

		// Return the number of bytes in each element of this tensor
		std::size_t elementSize() const noexcept;
		// Return all the dimensions as a array with 4 elements
		dim_t dims() const noexcept;
		// Return the dimension at index
		std::size_t dim(std::size_t index) const noexcept;
		// Return the width of the tensor, same as dim(0)
		std::size_t width() const noexcept;
		// Return the height of the tensor, same as dim(1)
		std::size_t height() const noexcept;
		// Return the number of dimensions larger than 1
		std::size_t numDims() const noexcept;

		// Attempt to change the dimensions of this tensor, without changing the order of the elements
		// Throws an exception if the number of elements would be changed by changing the dims
		void moddims(const dim_t & ndims);

		char* data() noexcept;
		const char* data() const noexcept;
	private:
		Type mtype;
		dim_t mdims;
		//range_t mrange;
		std::unique_ptr<char[]> mdata;

		void allocate();

		struct FillFunc {
			template<typename T1, typename T2>
			static void apply(char* _first, char* _last, const T2& val) {
				auto first = (T1*)_first;
				auto last = (T1*)_last;
				for (; first != last; ++first) {
					*first = static_cast<T1>(val);
				}
			}
		};
		struct AssignFunc {
			template<typename T1, typename InputIt>
			static void apply(char* _data_first, char* _data_last, InputIt first, InputIt last) {
				auto data_first = (T1*)_data_first;
				auto data_last = (T1*)_data_last;
				while (first != last && data_first != data_last) {
					*data_first = static_cast<T1>(*first);
					++data_first;
					++first;
				}
			}
		};
	public:
		struct iterator {
			using iterator_category = std::random_access_iterator_tag;
			using value_type = ValueRef;
			using reference = ValueRef;
			using pointer = ValueRef;
			using difference_type = std::ptrdiff_t;

			iterator(const iterator& other) noexcept = default;
			iterator& operator=(const iterator&) noexcept = default;
			~iterator() = default;

			iterator() noexcept;
			iterator(void * _data, ValueType _type) noexcept;
			
			bool operator==(const iterator & other) const noexcept;
			bool operator!=(const iterator& other) const noexcept;
			bool operator<(const iterator & other) const noexcept;
			bool operator<=(const iterator& other) const noexcept;
			bool operator>(const iterator& other) const noexcept;
			bool operator>=(const iterator& other) const noexcept;

			reference operator*() noexcept;
			pointer operator->() noexcept;
			reference operator[](difference_type offset) noexcept;

			iterator& operator++() noexcept;
			iterator operator++(int) noexcept;

			iterator& operator--() noexcept;
			iterator operator--(int) noexcept;

			iterator& operator+=(difference_type offset) noexcept;
			iterator& operator-=(difference_type offset) noexcept;

			iterator operator+(difference_type offset) const noexcept;
			iterator operator-(difference_type offset) const noexcept;

			difference_type operator-(iterator offset) const noexcept;
		private:
			friend struct const_iterator;
			std::size_t step;
			ValueType type;
			char * data;
		};

		struct const_iterator {
			using iterator_category = std::random_access_iterator_tag;
			using value_type = const ValueRef;
			using reference = const ValueRef;
			using pointer = const ValueRef;
			using difference_type = std::ptrdiff_t;

			const_iterator(const const_iterator&) noexcept = default;
			const_iterator& operator=(const const_iterator&) noexcept = default;
			~const_iterator() = default;


			const_iterator() noexcept;
			const_iterator(const void *, ValueType) noexcept;
			const_iterator(iterator other) noexcept;

			bool operator==(const const_iterator& other) const noexcept;
			bool operator!=(const const_iterator& other) const noexcept;
			bool operator<(const const_iterator& other) const noexcept;
			bool operator<=(const const_iterator& other) const noexcept;
			bool operator>(const const_iterator& other) const noexcept;
			bool operator>=(const const_iterator& other) const noexcept;

			reference operator*() noexcept;
			pointer operator->() noexcept;
			reference operator[](difference_type offset) noexcept;

			const_iterator& operator++() noexcept;
			const_iterator operator++(int) noexcept;

			const_iterator& operator--() noexcept;
			const_iterator operator--(int) noexcept;

			const_iterator& operator+=(difference_type offset) noexcept;
			const_iterator& operator-=(difference_type offset) noexcept;

			const_iterator operator+(difference_type offset) const noexcept;
			const_iterator operator-(difference_type offset) const noexcept;

			difference_type operator-(const_iterator offset) const noexcept;
		private:
			std::size_t step;
			ValueType type;
			const char* data;
		};
	};

	using ValueType = Value::Type;
}

template<typename T, gdev::ValueType = gdev::ValueTypeTraits<T>::id>
bool operator==(const T& lh, const gdev::Value& rh) {
	return rh == lh;
}
template<typename T, gdev::ValueType = gdev::ValueTypeTraits<T>::id>
bool operator!=(const T& lh, const gdev::Value& rh) {
	return rh != lh;
}

std::ostream& operator<<(std::ostream&os, const gdev::Value & value);