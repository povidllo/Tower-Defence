#pragma once
#include "EffectSample.h"

namespace TDEngine::Inner {

	class EffectOnEnemySample : public EffectSample {
	public:
		double initialHealth = 0.0;      // absolute change
		double initialSpeedPercent = 0.0; // -100..inf, 0 = no change
		double periodicHealth = 0.0;
		double periodicSpeedPercent = 0.0;
	};

} // namespace TDEngine::Inner