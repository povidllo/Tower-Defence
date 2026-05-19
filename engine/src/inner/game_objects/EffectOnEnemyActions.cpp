#include "EffectOnEnemyActions.h"
#include "EnemyActions.h"
#include "../core/EngineStorage.h"

namespace TDEngine::Inner {

	EffectOnEnemyActions::EffectOnEnemyActions(EffectOnEnemySample sample, std::shared_ptr<EnemyActions> target)
		: storage(std::move(sample)) {
		storage.target = std::move(target);
		storage.isFinished = false;
		storage.elapsedTime = 0.0;
		storage.timeSinceLastPeriod = 0.0;
		storage.periodsDone = 0;
		storage.initialApplied = false;
	}

	void EffectOnEnemyActions::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {
		if (storage.isFinished || !storage.target->storage.isAlive) {
			storage.isFinished = true;
			return;
		}

		double dt = timePassedMillis / 1000.0;
		storage.elapsedTime += dt;

		// Apply initial effect once
		if (!storage.initialApplied) {
			storage.target->storage.currentHP += storage.initialHealth;
			storage.target->storage.curSpeed -= storage.target->storage.getSpeed() * (storage.initialSpeedPercent / 100.0);
			storage.initialApplied = true;
		}

		// Periodic effect
		if (storage.period > 0.0) {
			storage.timeSinceLastPeriod += dt;
			while (storage.timeSinceLastPeriod >= storage.period) {
				storage.target->storage.currentHP += storage.periodicHealth;
				storage.target->storage.curSpeed -= storage.target->storage.getSpeed() * (storage.periodicSpeedPercent / 100.0);
				storage.timeSinceLastPeriod -= storage.period;
				storage.periodsDone++;
			}
		}

		// Check duration
		if (storage.elapsedTime >= storage.duration) {
			end();
		}
	}

	void EffectOnEnemyActions::end() {
		storage.target->storage.curSpeed += storage.target->storage.getSpeed() * (storage.initialSpeedPercent / 100.0);
		storage.target->storage.curSpeed += storage.periodsDone * storage.target->storage.getSpeed() * (storage.periodicSpeedPercent / 100.0);
		// Apply on-end effects (create new effect actions)
		// for (const auto& effectName : storage.onEndEffects) {
		// }
		storage.isFinished = true;
	}

} // namespace TDEngine::Inner