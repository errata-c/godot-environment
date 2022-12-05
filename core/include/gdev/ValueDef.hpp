#pragma once
#include <gdev/Value.hpp>
#include <cinttypes>
#include <limits>
#include <array>

namespace gdev {
	using range_t = std::array<double, 2>;

	/*
	Defines a value for using the SpaceDef class.
	The full definition of a value for use in SpaceDef includes:
		The underlying type of the value (Int, Bool, or Real)
		The range of accepted values
		The size of the (4 dimensional) tensor
	*/
	class ValueDef {
	public:
		ValueDef() noexcept;

		ValueDef(const ValueDef &) = default;
		ValueDef & operator=(const ValueDef &) = default;
		~ValueDef() = default;

		static constexpr range_t RealRange() {
			return {-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() };
		}
		static constexpr range_t BoolRange() {
			return { 0.0, 1.0 };
		}
		static constexpr range_t IntRange() {
			return { std::numeric_limits<int>::min(), std::numeric_limits<int>::max() };
		}
	
		static ValueDef MakeBool(dim_t dims);

		static ValueDef MakeReal(
			dim_t dims,
			double low,
			double high);
		static ValueDef MakeReal(
			dim_t dims,
			range_t range = RealRange());

		static ValueDef MakeInt(
			dim_t dims,
			int low,
			int high);
		static ValueDef MakeInt(
			dim_t dims,
			range_t range = IntRange());

		ValueType type() const noexcept;

		double lowerBound() const noexcept;
		double upperBound() const noexcept;
		range_t bounds() const noexcept;
		range_t range() const noexcept;

		bool contains(bool val) const noexcept;
		bool contains(int val) const noexcept;
		bool contains(double val) const noexcept;
		bool contains(const Value & val) const noexcept;

		bool isBool() const noexcept;
		bool isReal() const noexcept;
		bool isInt() const noexcept;

		std::size_t size() const noexcept;
		const dim_t& dims() const noexcept;
		int dim(int index) const noexcept;

		bool operator==(const ValueDef& other) const noexcept;
		bool operator!=(const ValueDef& other) const noexcept;

		Value instance() const;
	private:
		ValueDef(ValueType _type, dim_t _dims, double _low, double _high) noexcept;

		ValueType mtype;
		dim_t mdims;
		union {
			range_t mrange;
			struct {
				double low, high;
			};
		};
	};
}