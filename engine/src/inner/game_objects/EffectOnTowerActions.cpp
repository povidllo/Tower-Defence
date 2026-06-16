#include "EffectOnTowerActions.h"

#include <set>

#include "../core/EngineStorage.h"
#include "TowerActions.h"

namespace TDEngine::Inner {

	EffectOnTowerActions::EffectOnTowerActions(TowerEffectSample sample, std::shared_ptr<TowerActions> target)
	: storage(std::move(sample)), MapObject(sample.getVisualTexturePath(),
		target->positionCoordinates.first, target->positionCoordinates.second, MapObjectTypes::Effect) {
		storage.target = target;
		storage.isFinished = false;
		storage.elapsedTime = 0.0;
		storage.timeSinceLastPeriod = 0.0;
		storage.periodsDone = 0;
		storage.initialApplied = false;
	}

	void EffectOnTowerActions::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {
		if (storage.isFinished) {
			storage.isFinished = true;
			return;
		}

		positionCoordinates.first = storage.target->positionCoordinates.first;
		positionCoordinates.second = storage.target->positionCoordinates.second;

		double dt = timePassedMillis / 1000.0;
		storage.elapsedTime += dt;

		// Apply initial effect once
		if (!storage.initialApplied) {
			storage.target->storage.curDamage += storage.getStartDamageFlatImpact();
			storage.target->storage.curDamage += storage.target->storage.getDamage() * (storage.getStartDamagePercentImpact() / 100.0);
			storage.target->storage.curFireRate *= storage.getStartAttackSpeedPercentImpact() / 100.0;
			storage.initialApplied = true;
		}

		// Periodic effect
		if (storage.getPeriodSeconds() > 0.0) {
			storage.timeSinceLastPeriod += dt;
			while (storage.timeSinceLastPeriod >= storage.getPeriodSeconds()) {
				storage.target->storage.curDamage += storage.getPeriodicDamageFlatImpact();
				storage.target->storage.curDamage += storage.target->storage.getDamage() * (storage.getPeriodicDamagePercentImpact() / 100.0);
				storage.target->storage.curFireRate *= storage.getPeriodicAttackSpeedPercentImpact()/100.0;
				storage.timeSinceLastPeriod -= storage.getPeriodSeconds();
				storage.periodsDone++;
			}
		}

		// Check duration
		if (storage.elapsedTime >= storage.getDurationSeconds()) {
			end(engineStorage);
		}
	}

	void EffectOnTowerActions::end(std::shared_ptr<EngineStorage> engineStorage) {
		storage.target->storage.curDamage -= storage.getStartDamageFlatImpact();
		storage.target->storage.curDamage -= storage.target->storage.getDamage() * (storage.getStartDamagePercentImpact() / 100.0);
		storage.target->storage.curFireRate /= storage.getStartAttackSpeedPercentImpact() / 100.0;
		storage.target->storage.curDamage -= storage.periodsDone * storage.getPeriodicDamageFlatImpact();
		storage.target->storage.curDamage -= storage.periodsDone * storage.target->storage.getDamage() * (storage.getPeriodicDamagePercentImpact() / 100.0);
		for (int i = 0; i < storage.periodsDone; i++) {
			storage.target->storage.curFireRate /= storage.getPeriodicAttackSpeedPercentImpact() / 100.0;
		}
		applyEffects(storage.getEffectsAfterFinish(), engineStorage);
		storage.isFinished = true;
	}
	void EffectOnTowerActions::applyEffects(const std::vector<std::string>& effectNames,
							  std::shared_ptr<EngineStorage> engineStorage) {
		std::set<std::string> effectsNamesSet;
		for (auto name : effectNames) {
			effectsNamesSet.insert(name);
		}

		std::vector<std::shared_ptr<TowerEffectSample>> towerEffects;

		for (const auto& effectSample : engineStorage->curProject->getEffects()) {
			if (effectsNamesSet.count(effectSample->getName()) > 0) {
				if (effectSample->getKind() == EffectSample::Kind::Tower) {
					towerEffects.push_back(std::static_pointer_cast<TowerEffectSample> (effectSample));
				}
			}
		}

		if (!towerEffects.empty()) {
			for (auto effectSample : towerEffects) {
				auto newEffect = std::make_shared<EffectOnTowerActions>(*effectSample, storage.target);
				engineStorage->addEffectOnTower(newEffect);
			}
		}
	}
} // namespace TDEngine::Inner