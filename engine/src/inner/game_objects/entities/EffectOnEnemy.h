#pragma once
#include <memory>

#include "../EnemyActions.h"
#include "EnemyEffectSample.h"
namespace TDEngine {
	namespace Inner {
		class EffectOnEnemy : public EnemyEffectSample{
		public:
			explicit EffectOnEnemy(EnemyEffectSample sample)
			: EnemyEffectSample(std::move(sample)) {}

			std::shared_ptr<EnemyActions> target;
			bool isFinished = false;
			double elapsedTime = 0.0;      // seconds
			double timeSinceLastPeriod = 0.0;
			int periodsDone = 0;
			bool initialApplied = false;
		};
	} // Inner
} // TDEngine