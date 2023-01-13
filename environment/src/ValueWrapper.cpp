#include <gdev/ValueWrapper.hpp>
#include <gdev/ValueIndexer.hpp>

#include "ValueUtils.hpp"

#include <algorithm>
#include <cmath>
#include <string>
#include <sstream>

namespace godot {
	void ValueWrapper::_register_methods() {
		register_method("vtype", &ValueWrapper::vtype);
		register_method("elements", &ValueWrapper::elements);
		register_method("dims", &ValueWrapper::dims);

		register_method("clear", &ValueWrapper::clear);
		register_method("reset", &ValueWrapper::reset);
		register_method("reset_uninitialized", &ValueWrapper::reset_uninitialized);

		register_method("_iter_init", &ValueWrapper::_iter_init);
		register_method("_iter_next", &ValueWrapper::_iter_next);
		register_method("_iter_get", &ValueWrapper::_iter_get);

		register_method("_to_string", &ValueWrapper::_to_string);

		register_method("get", &ValueWrapper::get);
		register_method("get_flat", &ValueWrapper::get_flat);

		register_method("set", &ValueWrapper::set);
		register_method("set_flat", &ValueWrapper::set_flat);

		register_method("assign", &ValueWrapper::assign);
		register_method("fill", &ValueWrapper::fill);
	}
	void ValueWrapper::_init() {
		current = 0;
		end = 0;
	}

	void ValueWrapper::clear() {
		data.clear();

		current = 0;
		end = elements();
	}
	void ValueWrapper::reset_uninitialized(Variant _dims, String _vtype) {
		auto cdims = gdev::to_dims(_dims);
		if (!cdims) {
			Godot::print_error("Failed to convert dimensions argument!", "reset_uninitialized", __FILE__, __LINE__);
			return;
		}

		auto parsed = gdev::to_value_type(_vtype);
		if (!parsed) {
			Godot::print_error(
				String("'{}' is not an accepted value type!").format(Array::make(_vtype)),
				"reset_uninitialized", __FILE__, __LINE__);
			return;
		}

		data = gdev::Value::Uninitialized(cdims.value(), parsed.value());

		current = 0;
		end = elements();
	}

	struct IterAssign {
		template<typename T>
		static bool apply(gdev::Value& lh, Object * obj) {
			if (!bool(obj->call("_iter_init"))) {
				return false;
			}
			T* write = (T*)lh.data();
			size_t i = 0, cap = lh.elements();

			if (i == cap) {
				return true;
			}

			do {
				auto res = gdev::convert_to<T>(obj->call("_iter_get"));
				if (!res) {
					return false;
				}
				*write++ = *res;

			} while (++i < cap && bool(obj->call("_iter_next")));

			return true;
		}
	};
	struct ArrayAssign {
		template<typename T>
		static bool apply(gdev::Value& lh, Array & arr) {
			T* write = (T*)lh.data();

			for (size_t i = 0, cap = std::min<size_t>(lh.elements(), arr.size()); i < cap; ++i) {
				 auto res = gdev::convert_to<T>(arr[i]);
				 if (!res) {
					return false;
				 }
				 *write++ = *res;
			}

			return true;
		}
	};
	template<typename B>
	struct ConstantAssign {
		template<typename T>
		static void apply(gdev::Value& lh, const B & val) {
			T* write = (T*)lh.data();

			for (size_t i = 0, cap = lh.elements(); i < cap; ++i) {
				*write++ = static_cast<T>(val);
			}
		}
	};
	template<typename Pool>
	struct PoolAssign {
		template<typename T>
		static void apply(gdev::Value& lh, const Pool& pool) {
			T* write = (T*)lh.data();
			auto read_obj = pool.read();
			auto read = read_obj.ptr();

			for (size_t i = 0, cap = std::min<size_t>(lh.elements(), pool.size()); i < cap; ++i) {
				*write = static_cast<T>(*read);

				++write;
				++read;
			}
		}
	};

	void ValueWrapper::reset(Variant _dims, String _vtype, Variant val) {
		// Check that the initializer object is valid
		auto cdims = gdev::to_dims(_dims);
		if (!cdims) {
			ERR_PRINT("Failed to convert dimensions argument!");
			return;
		}

		auto parsed = gdev::to_value_type(_vtype);
		if (!parsed) {
			ERR_PRINT(
				String("'{}' is not an accepted value type!").format(Array::make(_vtype))
				);
			return;
		}

		gdev::ValueType ctype = *parsed;

		data = gdev::Value::Uninitialized(cdims.value(), parsed.value());
		size_t ecount = data.elements();

		switch (val.get_type()) {
		case Variant::ARRAY: { // Array init
			Array arr(val);
			if (arr.size() != ecount) {
				WARN_PRINT("Mismatch between the size of Value and initializer Array.");
			}
			
			gdev::visitors::single_visit<ArrayAssign>(data.type(), data, arr);
			break;
		}
		case Variant::OBJECT: { // Possible iterator init
			Object * obj(val);
			if (obj->has_method("_iter_init") && obj->has_method("_iter_next") && obj->has_method("_iter_get")) {
				// Object is a valid iterator
				gdev::visitors::single_visit<IterAssign>(data.type(), data, obj);
			}
			else {
				ERR_PRINT("Cannot convert non-iterator object into a value!");
				return;
			}
			break;
		}
		case Variant::INT: { // Simple int value init
			gdev::visitors::single_visit<ConstantAssign<int64_t>>(data.type(), data, int64_t(val));
			break;
		}
		case Variant::REAL: { // Simple double value init
			gdev::visitors::single_visit<ConstantAssign<double>>(data.type(), data, double(val));
			break;
		}
		case Variant::BOOL: { // Simple bool value init
			gdev::visitors::single_visit<ConstantAssign<bool>>(data.type(), data, bool(val));
			break;
		}

		case Variant::POOL_INT_ARRAY: { // pool of integers
			PoolIntArray arr(val);
			gdev::visitors::single_visit<PoolAssign<PoolIntArray>>(data.type(), data, arr);
			break;
		}
		case Variant::POOL_REAL_ARRAY: { // pool of floats (single precision)
			PoolRealArray arr(val);
			gdev::visitors::single_visit<PoolAssign<PoolRealArray>>(data.type(), data, arr);
			break;
		}
		case Variant::POOL_BYTE_ARRAY: { // Raw value conversion? Or interpret as i8?
			PoolByteArray arr(val);
			gdev::visitors::single_visit<PoolAssign<PoolByteArray>>(data.type(), data, arr);
			break;
		}
		default:
			ERR_PRINT(
				String("Cannot convert {0} to required value type!").format(Array::make(gdev::to_godot_string(val.get_type())))
			);
			return;
		}
	}

	void ValueWrapper::assign(Variant val) {
		size_t ecount = elements();

		switch (val.get_type()) {
		case Variant::ARRAY: { // Array init
			Array arr(val);
			if (arr.size() != ecount) {
				WARN_PRINT("Mismatch between the size of Value and initializer Array.");
			}

			gdev::visitors::single_visit<ArrayAssign>(data.type(), data, arr);
			break;
		}
		case Variant::OBJECT: { // Possible iterator init
			Object* obj(val);
			if (obj->has_method("_iter_init") && obj->has_method("_iter_next") && obj->has_method("_iter_get")) {
				// Object is a valid iterator
				gdev::visitors::single_visit<IterAssign>(data.type(), data, obj);
			}
			else {
				ERR_PRINT("Cannot convert non-iterator object into a value!");
				return;
			}
			break;
		}

		case Variant::POOL_INT_ARRAY: { // pool of integers
			PoolIntArray arr(val);
			gdev::visitors::single_visit<PoolAssign<PoolIntArray>>(data.type(), data, arr);
			break;
		}
		case Variant::POOL_REAL_ARRAY: { // pool of floats (single precision)
			PoolRealArray arr(val);
			gdev::visitors::single_visit<PoolAssign<PoolRealArray>>(data.type(), data, arr);
			break;
		}
		case Variant::POOL_BYTE_ARRAY: { // Raw value conversion? Or interpret as i8?
			PoolByteArray arr(val);
			gdev::visitors::single_visit<PoolAssign<PoolByteArray>>(data.type(), data, arr);
			break;
		}
		default:
			ERR_PRINT(
				String("Cannot assign using {0} type!").format(Array::make(gdev::to_godot_string(val.get_type())))
			);
			return;
		}
	}
	void ValueWrapper::fill(Variant val) {
		switch (val.get_type()) {
		case Variant::INT: { // Simple int value init
			gdev::visitors::single_visit<ConstantAssign<int64_t>>(data.type(), data, int64_t(val));
			break;
		}
		case Variant::REAL: { // Simple double value init
			gdev::visitors::single_visit<ConstantAssign<double>>(data.type(), data, double(val));
			break;
		}
		case Variant::BOOL: { // Simple bool value init
			gdev::visitors::single_visit<ConstantAssign<bool>>(data.type(), data, bool(val));
			break;
		}
		default:
			ERR_PRINT(
				String("Cannot fill using {0} type!").format(Array::make(gdev::to_godot_string(val.get_type())))
			);
			return;
		}
	}

	bool ValueWrapper::_iter_init() {
		current = 0;
		end = data.elements();
		return current != end;
	}
	bool ValueWrapper::_iter_next() {
		++current;
		return current != end;
	}

	
	Variant ValueWrapper::_iter_get() {
		if (current < end) {
			return gdev::visitors::single_visit<VariantCast>(data.type(), data[current]);
		}
		else {
			ERR_PRINT("Attempt to access iterator out of range");
			return {};
		}
	}

	void ValueWrapper::set(size_t i0, size_t i1, size_t i2, size_t i3, Variant var) {
		const auto & d = data.dims();
		ERR_FAIL_INDEX(i0, d[0]);
		ERR_FAIL_INDEX(i1, d[1]);
		ERR_FAIL_INDEX(i2, d[2]);
		ERR_FAIL_INDEX(i3, d[3]);

		size_t idx = i0 + i1 * d[0] + i2 * d[0] * d[1] + i3 * d[0] * d[1] * d[2];
		
		gdev::visitors::single_visit<VariantAssign>(data.type(), data[idx], var);
	}
	void ValueWrapper::set_flat(size_t i, Variant var) {
		size_t e = elements();
		ERR_FAIL_INDEX(i, e);

		gdev::visitors::single_visit<VariantAssign>(data.type(), data[i], var);
	}

	Variant ValueWrapper::get(size_t i0, size_t i1, size_t i2, size_t i3) {
		const auto& d = data.dims();
		ERR_FAIL_INDEX_V(i0, d[0], Variant{});
		ERR_FAIL_INDEX_V(i1, d[1], Variant{});
		ERR_FAIL_INDEX_V(i2, d[2], Variant{});
		ERR_FAIL_INDEX_V(i3, d[3], Variant{});

		size_t idx = i0 + i1 * d[0] + i2 * d[0] * d[1] + i3 * d[0] * d[1] * d[2];

		return gdev::visitors::single_visit<VariantCast>(data.type(), data[idx]);
	}
	Variant ValueWrapper::get_flat(size_t i) {
		size_t e = elements();
		ERR_FAIL_INDEX_V(i, e, Variant{});

		return gdev::visitors::single_visit<VariantCast>(data.type(), data[i]);
	}

	
	ValueIndexer* ValueWrapper::index(Variant i0, Variant i1, Variant i2, Variant i3) {
		return nullptr;
	}

	ValueIndexer* ValueWrapper::slice(Variant idx) {
		return nullptr;
	}
	ValueIndexer* ValueWrapper::row(Variant idx) {
		return nullptr;
	}
	ValueIndexer* ValueWrapper::col(Variant idx) {
		return nullptr;
	}

	Array ValueWrapper::dims() const {
		auto d = data.dims();
		return Array::make(d[0], d[1], d[2], d[3]);
	}
	size_t ValueWrapper::elements() const noexcept {
		return data.elements();
	}

	String ValueWrapper::vtype() const {
		return gdev::to_string_view(data.type()).data();
	}

	struct StringGen {
		template<typename T>
		static std::string apply(const gdev::Value& lh) {
			const T* read = (const T*)lh.data();
			const T* end = read + lh.elements();

			std::stringstream ss;
			ss << "[ ";
			if (read != end) {
				if constexpr (sizeof(T) == 1) {
					ss << int(*read);
				}
				else {
					ss << *read;
				}
				
				++read;
			}
			while (read != end) {
				if constexpr (sizeof(T) == 1) {
					ss << ", " << int(*read);
				}
				else {
					ss << ", " << *read;
				}

				++read;
			}
			ss << " ]";

			return ss.str();
		}
	};
	String ValueWrapper::_to_string() const {
		std::string tmp = gdev::visitors::single_visit<StringGen>(data.type(), data);
		return String(tmp.c_str());
	}
}