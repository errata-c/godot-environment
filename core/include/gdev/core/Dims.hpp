#pragma once
#include <cinttypes>
#include <algorithm>
#include <cmath>
#include <array>

namespace gdev {
	class Dims: public std::array<size_t, 4> {
	public:
		using parent_t = std::array<size_t, 4>;

		Dims(const Dims&) noexcept = default;
		Dims& operator=(const Dims&) noexcept = default;

		Dims()
			: parent_t{0, 0, 0, 0}
		{}
		Dims(size_t d0, size_t d1 = 1, size_t d2 = 1, size_t d3 = 1)
			: parent_t{d0, d1, d2, d3}
		{}

		size_t elements() const noexcept {
			return (*this)[0] * (*this)[1] * (*this)[2] * (*this)[3];
		}
		size_t ndims() const noexcept {
			size_t count = 0;
			for (size_t d: *this) {
				if (d <= 0) {
					return 0;
				}
				else if (d > 1) {
					++count;
				}
			}
			return count;
		}
	};

	using dims_t = Dims;
}