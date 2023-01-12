#pragma once
#include <cstddef>
#include <cinttypes>
#include <cassert>
#include <stdexcept>

#include <string>
#include <string_view>
#include <iosfwd>

namespace gdev {
	enum class ValueType {
		b8, // boolean

		i8, // 8 bit integer
		i16, // 16 bit integer
		i32, // 32 bit integer
		i64, // 64 bit integer

		f32, // 32 bit floating point
		f64, // 64 bit floating point
	};

	constexpr bool is_floating_point(ValueType id) noexcept {
		return 
			static_cast<int>(id) >= static_cast<int>(ValueType::f32) &&
			static_cast<int>(id) <= static_cast<int>(ValueType::f64);
	}
	constexpr bool is_integer(ValueType id) noexcept {
		return
			static_cast<int>(id) >= static_cast<int>(ValueType::i8) &&
			static_cast<int>(id) <= static_cast<int>(ValueType::i64);
	}

	inline std::size_t SizeOf(ValueType type) noexcept {
		switch (type) {
		case ValueType::b8:
			return sizeof(bool);

		case ValueType::i8:
			return sizeof(int8_t);
		case ValueType::i16:
			return sizeof(int16_t);
		case ValueType::i32:
			return sizeof(int32_t);
		case ValueType::i64:
			return sizeof(int64_t);

		case ValueType::f32:
			return sizeof(float);
		case ValueType::f64:
			return sizeof(double);

		default:
			assert(false);
			return 0;
		}
	}

	std::string to_string(ValueType id);
	std::string_view to_string_view(ValueType id) noexcept;

	template<typename T>
	struct ValueTypeTraits {};

	template<>
	struct ValueTypeTraits<bool> {
		static constexpr ValueType id = ValueType::b8;
	};

	template<>
	struct ValueTypeTraits<int8_t> {
		static constexpr ValueType id = ValueType::i8;
	};
	template<>
	struct ValueTypeTraits<int16_t> {
		static constexpr ValueType id = ValueType::i16;
	};
	template<>
	struct ValueTypeTraits<int32_t> {
		static constexpr ValueType id = ValueType::i32;
	};
	template<>
	struct ValueTypeTraits<int64_t> {
		static constexpr ValueType id = ValueType::i64;
	};

	template<>
	struct ValueTypeTraits<float> {
		static constexpr ValueType id = ValueType::f32;
	};
	template<>
	struct ValueTypeTraits<double> {
		static constexpr ValueType id = ValueType::f64;
	};


	template<typename T>
	constexpr T epsilon() {
		if (std::is_same_v<T, float>) {
			return 1e-5;
		}
		else {
			return 1e-15;
		}
	}

	namespace visitors {
		// static function visitor function
		template<typename Func, typename... Ts>
		decltype(auto) single_visit(ValueType id, Ts&&... args) {
			switch (id) {
			case ValueType::b8:
				return Func::apply<bool>( std::forward<Ts>(args)...);

			case ValueType::i8:
				return Func::apply<int8_t>( std::forward<Ts>(args)...);
			case ValueType::i16:
				return Func::apply<int16_t>( std::forward<Ts>(args)...);
			case ValueType::i32:
				return Func::apply<int32_t>( std::forward<Ts>(args)...);
			case ValueType::i64:
				return Func::apply<int64_t>( std::forward<Ts>(args)...);

			case ValueType::f32:
				return Func::apply<float>( std::forward<Ts>(args)...);
			case ValueType::f64:
				return Func::apply<double>( std::forward<Ts>(args)...);

			default:
				throw std::logic_error("Unreachable!");
			}
		}

		template<typename Func, typename A>
		struct DoubleVisit2 {
			template<typename B, typename... Ts>
			static decltype(auto) apply(Ts&&... args) {
				return Func::apply<A, B>(std::forward<Ts>(args)...);
			}
		};
		template<typename Func>
		struct DoubleVisit1 {
			template<typename A, typename... Ts>
			static decltype(auto) apply(ValueType id2, Ts&&... args) {
				return single_visit<DoubleVisit2<Func, A>>(id2, std::forward<Ts>(args)...);
			}
		};
		template<typename Func, typename... Ts>
		decltype(auto) double_visit(ValueType id, ValueType id2, Ts&&... args) {
			return single_visit<DoubleVisit1<Func>>(id, id2, std::forward<Ts>(args)...);
		}
	}
}

std::ostream & operator<<(std::ostream& os, gdev::ValueType id);