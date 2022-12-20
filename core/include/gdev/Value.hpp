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
			: Value(val, dim_t{1,1,1,1}, id)
		{}

		// Single value dimensioned constructor
		template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
		Value(const T& val, dim_t dims, ValueType id = ValueTypeTraits<T>::id)
			: mtype(id)
			, mdims{ dims }
		{
			allocate();
			fill(val);
		}

		// flat initializer list constructor
		template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
		Value(std::initializer_list<T> init, ValueType id = ValueTypeTraits<T>::id)
			: Value(init, dim_t{init.size(), 1, 1, 1}, id)
		{}

		// Dimensioned initializer list constructor
		template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
		Value(std::initializer_list<T> init, dim_t dims, ValueType id = ValueTypeTraits<T>::id)
			: mtype(id)
			, mdims(dims)
		{
			allocate();

		}

		template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
		void fill(const T& val) {
			visitors::single_visit<FillFunc>(type(), data(), data() + bytes(), val);
		}

		std::size_t size() const noexcept;
		std::size_t elements() const noexcept;
		std::size_t bytes() const noexcept;

		Type type() const noexcept;

		ValueRef at(int i0);
		ConstValueRef at(int i0) const;

		ValueRef at(int i0, int i1);
		ConstValueRef at(int i0, int i1) const;

		ValueRef at(int i0, int i1, int i2);
		ConstValueRef at(int i0, int i1, int i2) const;

		ValueRef at(int i0, int i1, int i2, int i3);
		ConstValueRef at(int i0, int i1, int i2, int i3) const;

		ValueRef operator[](int index);
		ConstValueRef operator[](int index) const;

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
		int dim(int index) const noexcept;
		// Return the width of the tensor, same as dim(0)
		int width() const noexcept;
		// Return the height of the tensor, same as dim(1)
		int height() const noexcept;
		// Return the number of dimensions larger than 1
		int numDims() const noexcept;

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
				for (; first != last && data_first != data_last; ++data_first, ++first) {
					*data_first = static_cast<T1>(*first);
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
			int step;
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
			int step;
			ValueType type;
			const char* data;
		};
	};

	using ValueType = Value::Type;
}

bool operator==(bool lh, const gdev::Value& rh) noexcept;
bool operator!=(bool lh, const gdev::Value& rh) noexcept;

bool operator==(int lh, const gdev::Value& rh) noexcept;
bool operator!=(int lh, const gdev::Value& rh) noexcept;

bool operator==(double lh, const gdev::Value& rh) noexcept;
bool operator!=(double lh, const gdev::Value& rh) noexcept;