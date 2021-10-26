#include <gdev/Step.hpp>

namespace gdev {
	Step::Step(const Space& _obs, double _reward, bool _done) noexcept
		: mobs(&_obs)
		, mreward(_reward)
		, mdone(_done)
	{}

	const Space& Step::observation() const noexcept {
		return *mobs;
	}
	double Step::reward() const noexcept {
		return mreward;
	}
	bool Step::done() const noexcept {
		return mdone;
	}
}