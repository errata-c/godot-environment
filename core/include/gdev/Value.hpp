#pragma once
#include <gdev/ValueType.hpp>
#include <gdev/ValueRef.hpp>

#include <cinttypes>
#include <cstddef>
#include <vector>
#include <array>

// Potential alternative to Godot::Variant
// More specialized
namespace gdev {
	using dim_t = std::array<int, 4>;

	class Value {
	public:
		struct iterator;
		struct const_iterator;

		using Type = ValueType;

		explicit Value(bool val) noexcept;
		explicit Value(int val) noexcept;
		explicit Value(double val) noexcept;

		Value() noexcept;
		Value(const Value & other);
		Value& operator=(const Value& other);
		Value(Value && other) noexcept;
		Value& operator=(Value && other) noexcept;
		~Value();
		
		static Value MakeBool(bool val, int x = 1, int y = 1, int z = 1, int w = 1);
		static Value MakeReal(double val, int x = 1, int y = 1, int z = 1, int w = 1);
		static Value MakeInt(int val, int x = 1, int y = 1, int z = 1, int w = 1);

		static Value MakeBool(bool val, dim_t dims);
		static Value MakeReal(double val, dim_t dims);
		static Value MakeInt(int val, dim_t dims);

		std::size_t size() const noexcept;

		Type type() const noexcept;

		bool isBool() const noexcept;
		bool isReal() const noexcept;
		bool isInt() const noexcept;
		
		bool & asBool();
		const bool& asBool() const;

		double& asReal();
		const double& asReal() const;

		int& asInt();
		const int& asInt() const;

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

		bool operator==(const gdev::Value& rh) const noexcept;
		bool operator!=(const gdev::Value& rh) const noexcept;

		bool operator==(bool rh) const noexcept;
		bool operator!=(bool rh) const noexcept;

		bool operator==(int rh) const noexcept;
		bool operator!=(int rh) const noexcept;

		bool operator==(double rh) const noexcept;
		bool operator!=(double rh) const noexcept;

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
	private:
		Type mtype;
		dim_t mdims;
		void * data;

		union {
			bool binary;
			double real;
			int category;

			char buffer[sizeof(double)];
		};

		Value(Type _type, dim_t _dims, bool val);
		Value(Type _type, dim_t _dims, int val);
		Value(Type _type, dim_t _dims, double val);

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

			explicit operator bool*();
			explicit operator int*();
			explicit operator double*();

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

			explicit operator const bool* ();
			explicit operator const int* ();
			explicit operator const double* ();

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