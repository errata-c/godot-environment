#pragma once
#include "Value.hpp"
#include <cinttypes>
#include <limits>

namespace gdev {
	class ValueDef {
	public:
		ValueDef() noexcept;

		ValueDef(const ValueDef &) = default;
		ValueDef & operator=(const ValueDef &) = default;
		~ValueDef() = default;
	
		static ValueDef MakeBool(dim_t dims);

		static ValueDef MakeReal(
			dim_t dims,
			double low = -std::numeric_limits<double>::infinity(),
			double high = std::numeric_limits<double>::infinity());

		static ValueDef MakeInt(
			dim_t dims,
			int low = std::numeric_limits<int>::min(),
			int high = std::numeric_limits<int>::max());

		ValueType type() const noexcept;

		double lowerBound() const noexcept;
		double upperBound() const noexcept;

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

		Value instance() const;
	private:
		ValueDef(ValueType _type, dim_t _dims, double _low, double _high) noexcept;

		ValueType mtype;
		dim_t mdims;
		double low, high;
	};
}