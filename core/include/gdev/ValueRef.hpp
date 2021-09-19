#pragma once
#include "ValueType.hpp"

namespace gdev {
	class ConstValueRef;

	// Dynamic reference to a datum in the value class
	// Constructor creates a new reference, assignment assigns to the referenced object.
	class ValueRef {
	public:
		friend class ConstValueRef;

		~ValueRef() = default;

		ValueRef(bool& val) noexcept;
		ValueRef(int& val) noexcept;
		ValueRef(double& val) noexcept;
		ValueRef(const ValueRef& val) noexcept;
		ValueRef(void* val, ValueType _type) noexcept;

		operator bool();
		operator int();
		operator double();

		bool operator==(bool val) const noexcept;
		bool operator==(int val) const noexcept;
		bool operator==(double val) const noexcept;
		bool operator==(const ValueRef& val) const noexcept;

		bool operator!=(bool val) const noexcept;
		bool operator!=(int val) const noexcept;
		bool operator!=(double val) const noexcept;
		bool operator!=(const ValueRef& val) const noexcept;

		ValueRef& operator=(bool val);
		ValueRef& operator=(int val);
		ValueRef& operator=(double val);
		ValueRef& operator=(const ValueRef& val);

		bool isBool() const noexcept;
		bool isReal() const noexcept;
		bool isInt() const noexcept;

		bool & asBool();
		const bool & asBool() const;

		int & asInt();
		const int & asInt() const;

		double& asReal();
		const double& asReal() const;

		ValueType type() const noexcept;
	private:
		ValueType mtype;
		void* data;
	};


	class ConstValueRef {
	public:
		~ConstValueRef() = default;

		ConstValueRef(const bool& val) noexcept;
		ConstValueRef(const int& val) noexcept;
		ConstValueRef(const double& val) noexcept;
		ConstValueRef(const ConstValueRef& val) noexcept;
		ConstValueRef(const ValueRef& val) noexcept;
		ConstValueRef(const void* val, ValueType _type) noexcept;

		operator bool();
		operator int();
		operator double();

		bool operator==(bool val) const noexcept;
		bool operator==(int val) const noexcept;
		bool operator==(double val) const noexcept;
		bool operator==(const ConstValueRef& val) const noexcept;
		bool operator==(const ValueRef& val) const noexcept;

		bool operator!=(bool val) const noexcept;
		bool operator!=(int val) const noexcept;
		bool operator!=(double val) const noexcept;
		bool operator!=(const ConstValueRef& val) const noexcept;
		bool operator!=(const ValueRef& val) const noexcept;

		bool isBool() const noexcept;
		bool isReal() const noexcept;
		bool isInt() const noexcept;

		const bool& asBool() const;
		const int& asInt() const;
		const double& asReal() const;

		ValueType type() const noexcept;
	private:
		ValueType mtype;
		const void* data;
	};
}