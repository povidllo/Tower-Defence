// #pragma once
// #include "IActing.h"
// #include "samples/EffectOnTowerSample.h"
// #include <memory>
//
// namespace TDEngine::Inner {
// 	class EngineStorage;
// 	class TowerActions;
//
// 	class EffectOnTowerActions : public IActing {
// 	public:
// 		EffectOnTowerActions(EffectOnTowerSample sample, std::shared_ptr<TowerActions> target);
// 		void act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) override;
//
// 		EffectOnTowerSample storage;
// 		std::shared_ptr<TowerActions> target;
// 		bool isFinished = false;
//
// 	private:
// 		double elapsedTime = 0.0;
// 		double timeSinceLastPeriod = 0.0;
// 		bool initialApplied = false;
// 		double originalDamage = 0.0;
// 		double originalFireRate = 0.0;
// 	};
// } // namespace TDEngine::Inner