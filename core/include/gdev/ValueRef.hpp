#pragma once
#include <gdev/ValueType.hpp>
#include <type_traits>

namespace gdev {
	class ValueRef;
	class ConstValueRef {
	public:
		~ConstValueRef() = default;

		ConstValueRef(const ConstValueRef& val) noexcept;
		ConstValueRef(const ValueRef& val) noexcept;
		ConstValueRef(const void* val, ValueType _type) noexcept;

		bool operator==(const bool& val) const;
		bool operator==(const int8_t& val) const;
		bool operator==(const int16_t& val) const;
		bool operator==(const int32_t& val) const;
		bool operator==(const int64_t& val) const;
		bool operator==(const float& val) const;
		bool operator==(const double& val) const;

		template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
		bool operator!=(const T& val) const {
			return !operator==(val);
		}

		bool operator==(const ConstValueRef& val) const;
		bool operator==(const ValueRef& val) const;
		bool operator!=(const ConstValueRef& val) const;
		bool operator!=(const ValueRef& val) const;

		ValueType type() const noexcept;

		template<typename T>
		const T& as() const {
			if (ValueTypeTraits<T>::id == type()) {
				return _as<T>();
			}
			else {
				throw std::logic_error{ "error" };
			}
		}
	private:
		friend class ValueRef;

		// Unchecked version
		template<typename T>
		const T& _as() const {
			return *reinterpret_cast<const T*>(mdata);
		};

		ValueType mtype;
		const void* mdata;
	};

	// Dynamic reference to a datum in the value class
	// Constructor creates a new reference, assignment assigns to the referenced object.
	// Thus, it behaves similar to an actual reference, and the underlying pointer cannot be changed after initialization!
	class ValueRef {
	public:
		friend class ConstValueRef;

		~ValueRef() = default;

		ValueRef(const ValueRef& val) noexcept;
		ValueRef(void* val, ValueType _type) noexcept;

		template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
		bool operator==(const T& val) const {
			return ConstValueRef(*this) == val;
		}
		template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
		bool operator!=(const T& val) const {
			return ConstValueRef(*this) != val;
		}

		ValueRef& operator=(const bool& val);
		ValueRef& operator=(const int8_t& val);
		ValueRef& operator=(const int16_t& val);
		ValueRef& operator=(const int32_t& val);
		ValueRef& operator=(const int64_t& val);
		ValueRef& operator=(const float& val);
		ValueRef& operator=(const double& val);

		bool operator==(const ValueRef& val) const;
		bool operator!=(const ValueRef& val) const;
		ValueRef& operator=(const ValueRef& val);

		bool operator==(const ConstValueRef& val) const;
		bool operator!=(const ConstValueRef& val) const;
		ValueRef& operator=(const ConstValueRef& val);

		template<typename T>
		T& as() {
			if (ValueTypeTraits<T>::id == type()) {
				return _as<T>();
			}
			else {
				throw std::logic_error{ "error" };
			}
		}
		template<typename T>
		T& as() const {
			if (ValueTypeTraits<T>::id == type()) {
				return _as<T>();
			}
			else {
				throw std::logic_error{ "error" };
			}
		}

		ValueType type() const noexcept;
	private:
		ValueType mtype;
		void* mdata;

		// Unchecked version
		template<typename T>
		T& _as() {
			return *reinterpret_cast<T*>(mdata);
		};

		// Unchecked version
		template<typename T>
		T& _as() const {
			return *reinterpret_cast<T*>(mdata);
		};

		//struct compare_func
	};
}