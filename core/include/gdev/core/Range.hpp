#pragma once
#include <cinttypes>
#include <algorithm>
#include <cmath>

namespace gdev {
	// Simple class for an inclusive range of values
	template<typename T>
	class TRange {
		static_assert(std::is_arithmetic_v<T>, "gdev::TRange requires an arithmetic type!");

		static constexpr T Eps() {
			if constexpr (std::is_integral_v<T>) {
				return static_cast<T>(0);
			}
			else {
				if constexpr (std::is_same_v<scalar_t, float>) {
					return static_cast<T>(1e-5f);
				}
				else {
					return static_cast<T>(1e-10);
				}
			}
		}

		static constexpr bool is_float = std::is_floating_point_v<T>;
	public:
		using self_t = TRange<T>;

		TRange() noexcept
			: min{ T(0) }
			, max{ T(0) }
		{};
		TRange(const T& _min, const T& _max) noexcept
			: min(_min)
			, max(_max)
		{};

		~TRange() noexcept = default;
		TRange(const TRange&) noexcept = default;
		TRange(TRange&&) noexcept = default;
		TRange& operator=(const TRange&) noexcept = default;
		TRange& operator=(TRange&&) noexcept = default;

		const T& operator[](int i) const noexcept {
			assert(i == 0 || i == 1);
			if (i == 0) {
				return min;
			}
			else {
				return max;
			}
		}
		T& operator[](int i) noexcept {
			assert(i == 0 || i == 1);
			if (i == 0) {
				return min;
			}
			else {
				return max;
			}
		}

		T span() const noexcept {
			return max - min;
		}
		T size() const noexcept {
			return span();
		}
		T center() const noexcept {
			return (min + max) / T(2);
		}

		bool empty() const noexcept {
			if constexpr (is_float) {
				return (min - max) > Eps();
			}
			else {
				return max < min;
			}
		}
		bool valid() const noexcept {
			return !empty();
		}

		self_t& translate(const T& offset) noexcept {
			min += offset;
			max += offset;
			return *this;
		}
		self_t translated(const T& offset) const noexcept {
			self_t copy = *this;
			copy.translate(offset);
			return copy;
		}
		

		self_t& expand(const T& minv, const T& maxv) noexcept {
			min -= minv;
			max += maxv;
			return *this;
		};
		self_t& expand(const T& amount) noexcept {
			return expand(amount, amount);
		};
		self_t expanded(const T& amount) const noexcept {
			self_t copy = *this;
			return copy.expand(amount);
		};
		self_t expanded(const T& minv, const T& maxv) const noexcept {
			self_t copy = *this;
			return copy.expand(minv, maxv);
		};
		

		self_t& contract(const T& minv, const T& maxv) noexcept {
			min += minv;
			max -= maxv;
			T c = center();

			min = std::min(c, min);
			max = std::max(c, max);

			return *this;
		}
		self_t& contract(const T& amount) noexcept {
			return contract(amount, amount);
		}
		self_t contracted(const T& amount) const noexcept {
			self_t copy = *this;
			return copy.contract(amount);
		};
		self_t contracted(const T& minv, const T& maxv) const noexcept {
			self_t copy = *this;
			return copy.contract(minv, maxv);
		};

		self_t& merge(const T& point) const noexcept {
			min = std::min(min, point);
			max = std::max(max, point);
			return *this;
		}
		self_t& merge(const self_t& other) const noexcept {
			max = std::max(max, other.max);
			min = std::min(min, other.min);
		}

		self_t merged(const T & point) const noexcept {
			self_t copy = *this;
			copy.merge(point);
			return copy;
		}
		self_t merged(const self_t& other) const noexcept {
			self_t copy = *this;
			copy.merge(other);
			return copy;
		}

		

		bool contains(const T& point) const noexcept {
			if constexpr (is_float) {
				return
					(point - min) > -Eps() &&
					(point - max) < Eps();
			}
			else {
				return 
					min <= point &&
					point <= max;
			}
		};

		// Does not check if the ranges are valid prior to comparison
		bool contains(const self_t& other) const noexcept {
			if constexpr (is_float) {
				return
					(other.min - min) > -Eps() &&
					(other.max - max) < Eps();
			}
			else {
				return
					other.max <= max &&
					other.min >= min;
			}
		};

		bool overlaps(const self_t& other) const noexcept {
			if constexpr (is_float) {
				return
					(max - other.min) > -Eps() &&
					(other.max - min) > -Eps();
			}
			else {
				return
					max >= other.min &&
					other.max >= min;
			}
		}

		bool operator==(const self_t& other) const noexcept {
			if constexpr (is_float) {
				return 
					std::abs(other.min - min) < Eps() &&
					std::abs(other.max - max) < Eps();
			}
			else {
				return other.min == min && other.max == max;
			}
		}
		bool operator!=(const self_t& other) const noexcept {
			if constexpr (is_float) {
				return
					std::abs(other.min - min) > Eps() ||
					std::abs(other.max - max) > Eps();
			}
			else {
				return other.min != min || other.max != max;
			}
		}
	private:
		T min, max;
	};

	using range_t = TRange<double>;
}