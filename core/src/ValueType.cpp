#include <gdev/ValueType.hpp>
#include <ostream>

namespace gdev {
	template<std::size_t N>
	constexpr std::string_view MakeView(const char (&text)[N]) noexcept {
		return std::string_view(text, N-1);
	}
	
	std::string_view to_string_view(ValueType type) noexcept {
		switch (type) {
		case ValueType::b8:
			return MakeView("b8");

		case ValueType::i8:
			return MakeView("i8");
		case ValueType::i16:
			return MakeView("i16");
		case ValueType::i32:
			return MakeView("i32");
		case ValueType::i64:
			return MakeView("i64");

		case ValueType::f32:
			return MakeView("f32");
		case ValueType::f64:
			return MakeView("f64");

		default:
			assert(false);
			return MakeView("");
		}
	}

	std::string to_string(ValueType id) {
		return std::string(to_string_view(id));
	}
}

std::ostream& operator<<(std::ostream& os, gdev::ValueType id) {
	os << gdev::to_string_view(id);
	return os;
}