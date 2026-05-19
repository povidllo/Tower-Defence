#pragma once
#include <memory>
#include "../samples/EffectOnEnemySample.h"
#include "../EnemyActions.h"
namespace TDEngine {
	namespace Inner {
		class EnemyActions;
		class EffectOnEnemy : public EffectOnEnemySample{
		public:
			explicit EffectOnEnemy(EffectOnEnemySample sample)
			: EffectOnEnemySample(std::move(sample)) {}

			std::shared_ptr<EnemyActions> target;
			bool isFinished = false;
			double elapsedTime = 0.0;      // seconds
			double timeSinceLastPeriod = 0.0;
			int periodsDone = 0;
			bool initialApplied = false;
		};
	} // Inner
} // TDEngine