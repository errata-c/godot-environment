#pragma once
#include <gdev/GodotConversions.hpp>
#include <Variant.hpp>

namespace godot {
	struct VariantCast {
		template<typename T>
		static Variant apply(const gdev::ValueRef& value) {
			return Variant(value.as<T>());
		}
		template<typename T>
		static Variant apply(const gdev::ConstValueRef& value) {
			return Variant(value.as<T>());
		}
	};

	struct VariantAssign {
		template<typename T>
		static void apply(const gdev::ValueRef& ref, Variant value) {
			ref.as<T>() = T(value);
		}
	};
}