#pragma once
#include <memory>

#include "../../../../../editor/include/Entity/TowerEffectSample.h"
#include "../TowerActions.h"
namespace TDEngine {
	namespace Inner {
		class EffectOnTower : public TowerEffectSample{
		public:
			explicit EffectOnTower(TowerEffectSample sample)
			: TowerEffectSample(std::move(sample)) {}

			std::shared_ptr<TowerActions> target;
			bool isFinished = false;
			double elapsedTime = 0.0;      // seconds
			double timeSinceLastPeriod = 0.0;
			int periodsDone = 0;
			bool initialApplied = false;
		};
	} // Inner
} // TDEngine