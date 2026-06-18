#include "EffectOnEnemyActions.h"

#include <set>
#include <utility>

#include "../core/EngineStorage.h"
#include "EnemyActions.h"

namespace TDEngine::Inner {

	EffectOnEnemyActions::EffectOnEnemyActions(EnemyEffectSample sample,
        	std::vector<std::shared_ptr<EnginePlayer>> ownerPlayers, std::shared_ptr<EnemyActions> target)
		: storage(std::move(sample)), MapObject(sample.getVisualTexturePath(),
		target->positionCoordinates.first, target->positionCoordinates.second, MapObjectTypes::Effect) {
		storage.ownerPlayers = std::move(ownerPlayers);
		storage.target = target;
		storage.isFinished = false;
		storage.elapsedTime = 0.0;
		storage.timeSinceLastPeriod = 0.0;
		storage.periodsDone = 0;
		storage.initialApplied = false;

		std::cout << "[INFO] Created enemy effect: " << sample.getName() << " on target with xy: "
		<< target->positionCoordinates.first << " " << target->positionCoordinates.second << std::endl;
	}

	void EffectOnEnemyActions::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {
		if (storage.isFinished || !storage.target->storage.isAlive) {
			storage.isFinished = true;
			return;
		}

		positionCoordinates.first = storage.target->positionCoordinates.first;
		positionCoordinates.second = storage.target->positionCoordinates.second;

		double dt = timePassedMillis / 1000.0;
		storage.elapsedTime += dt;

		// Apply initial effect once
		if (!storage.initialApplied) {
			if (storage.getStartHealthImpact() < 0) {
				storage.target->storage.lastHitPlayers = std::move(storage.ownerPlayers);
			}
			storage.target->storage.currentHP += storage.getStartHealthImpact();
			storage.target->storage.curSpeed += storage.target->storage.getSpeed() * (storage.getStartSpeedImpactPercent() / 100.0);
			storage.initialApplied = true;
		}

		// Periodic effect
		if (storage.getPeriodSeconds() > 0.0) {
			storage.timeSinceLastPeriod += dt;
			while (storage.timeSinceLastPeriod >= storage.getPeriodSeconds()) {
				if (storage.getPeriodicHealthImpact() < 0) {
					storage.target->storage.lastHitPlayers = std::move(storage.ownerPlayers);
				}
				storage.target->storage.currentHP += storage.getPeriodicHealthImpact();
				storage.target->storage.curSpeed += storage.target->storage.getSpeed() * (storage.getStartSpeedImpactPercent() / 100.0);
				storage.timeSinceLastPeriod -= storage.getPeriodSeconds();
				storage.periodsDone++;
			}
		}

		// Check duration
		if (storage.elapsedTime >= storage.getDurationSeconds()) {
			end(engineStorage);
		}
	}

	void EffectOnEnemyActions::end(std::shared_ptr<EngineStorage> engineStorage) {
		storage.target->storage.curSpeed -= storage.target->storage.getSpeed() * (storage.getStartSpeedImpactPercent() / 100.0);
		storage.target->storage.curSpeed -= storage.periodsDone * storage.target->storage.getSpeed() * (storage.getPeriodicSpeedImpactPercent() / 100.0);
		applyEffects(storage.getEffectsAfterFinish(), engineStorage);
		storage.isFinished = true;
	}
	void EffectOnEnemyActions::applyEffects(const std::vector<std::string>& effectNames,
							  std::shared_ptr<EngineStorage> engineStorage) {
		std::set<std::string> effectsNamesSet;
		for (auto name : effectNames) {
			effectsNamesSet.insert(name);
		}

		std::vector<std::shared_ptr<EnemyEffectSample>> enemyEffects;

		for (const auto& effectSample : engineStorage->curProject->getEffects()) {
			if (effectsNamesSet.count(effectSample->getName()) > 0) {
				if (effectSample->getKind() == EffectSample::Kind::Enemy) {
					enemyEffects.push_back(std::static_pointer_cast<EnemyEffectSample> (effectSample));
				}
			}
		}

		if (!enemyEffects.empty()) {
			for (auto effectSample : enemyEffects) {
				auto newEffect = std::make_shared<EffectOnEnemyActions>(*effectSample, storage.ownerPlayers, storage.target);
				engineStorage->addEffectOnEnemy(newEffect);
			}
		}
	}

} // namespace TDEngine::Inner