#pragma once

#include <gdev/Space.hpp>

namespace gdev {
	class Step {
	public:
		Step(const Step &) = default;
		Step& operator=(const Step &) = default;
		~Step() = default;

		Step(const Space& _obs, double _reward, bool _done) noexcept;

		const Space& observation() const noexcept;
		double reward() const noexcept;
		bool done() const noexcept;
	private:
		const Space * mobs;
		double mreward;
		bool mdone;
	};
}