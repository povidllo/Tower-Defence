#include "EffectCreatorActions.h"

#include <set>
#include <utility>

#include "../core/EngineStorage.h"
#include "EnemyActions.h"
#include "TowerActions.h"

namespace TDEngine::Inner {

	EffectCreatorActions::EffectCreatorActions(std::string sampleName, std::shared_ptr<EngineStorage> engineStorage,
        	std::vector<std::shared_ptr<EnginePlayer>> ownerPlayers, std::shared_ptr<MapObject> attachedObject) :
		MapObject("", attachedObject->positionCoordinates, MapObjectTypes::EffectCreator), storage(*findSample(sampleName, engineStorage)) {
		storage.ownerPlayers = std::move(ownerPlayers);
		storage.isFinished = false;
		storage.initialApplied = false;
		storage.elapsedTime = 0.0;
		storage.timeSinceLastPeriod = 0.0;
		if (storage.getAttachmentPreference() == "point") {
			storage.attachedObject = std::make_shared<MapObject>(
					MapObject("", attachedObject->positionCoordinates, MapObjectTypes::Point));
		} else {
			if (attachedObject->type != MapObjectTypes::Point) {
				storage.attachedObject = attachedObject;
			}
			else {
				storage.attachedObject = findClosestTargetEnemy(engineStorage, attachedObject);
			}
		}

		std::cout << "[INFO] Createt EC: " << sampleName << " on target with xy: "
		<< attachedObject->positionCoordinates.first << " " << attachedObject->positionCoordinates.second << std::endl;
	}
	std::shared_ptr<EffectCreatorSample> EffectCreatorActions::findSample(std::string sampleName, std::shared_ptr<EngineStorage> engineStorage){
		for (auto ecSample: engineStorage->curProject->getEffectCreators()) {
			if (ecSample->getName() == sampleName) {
				return ecSample;
			}
		}
		return nullptr;
	}

	std::shared_ptr<MapObject> EffectCreatorActions::findClosestTargetEnemy(std::shared_ptr<EngineStorage> engineStorage,
		std::shared_ptr<MapObject> attachedObject) {
		std::shared_ptr<MapObject> closestTarget = nullptr;
		for (auto enemy : engineStorage->activeEnemies) {
			if (enemy->storage.isAlive && (closestTarget == nullptr ||
				attachedObject->getDistanceTo(enemy) < attachedObject->getDistanceTo(closestTarget))) {
				closestTarget = enemy;
			}
		}

		if (closestTarget != nullptr) {
			return closestTarget;
		}
		return attachedObject;
	}

	void EffectCreatorActions::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {
	    if (storage.isFinished) return;

	    double dt = timePassedMillis / 1000.0;
	    storage.elapsedTime += dt;

	    // If attached to an object that died, destroy self
	    if (storage.attachedObject->type == MapObjectTypes::Enemy) {
    		auto enemy = std::static_pointer_cast<EnemyActions>(storage.attachedObject);
    		if (!enemy->storage.isAlive) {
    			storage.isFinished = true;
    			return;
    		}
    		// Update position if attached to moving object
	        positionCoordinates = enemy->positionCoordinates;
	    }

	    // Apply initial effects once
	    if (!storage.initialApplied) {
	        applyEffects(storage.getStartEffects(), engineStorage);
    		storage.initialApplied = true;
	    }

	    // Periodic application
	    if (storage.getPeriodSeconds() > 0.0) {
	        storage.timeSinceLastPeriod += dt;
	        while (storage.timeSinceLastPeriod >= storage.getPeriodSeconds()) {
	            applyEffects(storage.getPeriodicEffects(), engineStorage);
	            storage.timeSinceLastPeriod -= storage.getPeriodSeconds();
	        }
	    }

	    if (storage.elapsedTime >= storage.getDurationSeconds()) {
	        storage.isFinished = true;
	    }
	}


	void EffectCreatorActions::applyEffects(const std::vector<std::string>& effectNames,
							  std::shared_ptr<EngineStorage> engineStorage) {
	std::cout << "[INFO] Trying to create effects by : " << storage.getName() << std::endl;
	std::cout << "[INFO] Trying to create effects owned : " << storage.ownerPlayers.size() << std::endl;
	std::set<std::string> effectsNamesSet;
	for (auto name : effectNames) {
		effectsNamesSet.insert(name);
	}

	std::vector<std::shared_ptr<EnemyEffectSample>> enemyEffects;
	std::vector<std::shared_ptr<TowerEffectSample>> towerEffects;

	for (const auto& effectSample : engineStorage->curProject->getEffects()) {
		if (effectsNamesSet.count(effectSample->getName()) > 0) {
			if (effectSample->getKind() == EffectSample::Kind::Enemy) {
				enemyEffects.push_back(std::static_pointer_cast<EnemyEffectSample> (effectSample));
			}
			else if (effectSample->getKind() == EffectSample::Kind::Tower) {
				towerEffects.push_back(std::static_pointer_cast<TowerEffectSample> (effectSample));
			}
		}
	}

	std::cout << "[INFO] Found effects amount " << enemyEffects.size() << " " << towerEffects.size() << std::endl;

	if (!enemyEffects.empty()) {
		for (const auto& target : getTargetEnemiesInRadius(engineStorage)) {
			for (auto effectSample : enemyEffects) {
				if (!effectSample->isStackable()) {
					bool b = false;
					for (auto effect : engineStorage->activeEnemyEffects) {
						if (effect->storage.target.get() == target.get()
							&& effect->storage.getName() == effectSample->getName()) {
							b = true;
							std::cout << "[INFO] stackable flag " << std::endl;
							break;
						}
					}
					if (b) continue;
				}
				auto newEffect = std::make_shared<EffectOnEnemyActions>(*effectSample, storage.ownerPlayers, target);
				engineStorage->addEffectOnEnemy(newEffect);
			}
		}
	}
	if (!towerEffects.empty()) {
		for (auto target : getTargetTowersInRadius(engineStorage)) {
			for (auto effectSample : towerEffects) {
				if (!effectSample->isStackable()) {
					bool b = false;
					for (auto effect : engineStorage->activeTowerEffects) {
						if (effect->storage.target.get() == target.get()
							&& effect->storage.getName() == effectSample->getName()) {
							b = true;
							break;
						}
					}
					if (b) continue;
				}
				auto newEffect = std::make_shared<EffectOnTowerActions>(*effectSample, storage.ownerPlayers, target);
				engineStorage->addEffectOnTower(newEffect);
			}
		}
	}
}

	std::vector<std::shared_ptr<EnemyActions>> EffectCreatorActions::getTargetEnemiesInRadius(std::shared_ptr<EngineStorage> engineStorage) {
	std::cout << "[INFO] Trying to find enemies for ec : " << storage.getName() << std::endl;
	std::vector<std::shared_ptr<EnemyActions>> result;
	if (storage.getRadius() <= 0.0) {
		if (storage.attachedObject->type == MapObjectTypes::Enemy) { // Or else nothing happens
			std::cout << "[INFO] Found single enemy (x, y) : " << storage.attachedObject->positionCoordinates.first
			<< " " << storage.attachedObject->positionCoordinates.second << std::endl;
			result.push_back(std::static_pointer_cast<EnemyActions>(storage.attachedObject));
		}
		return result;
	}

	for (auto& enemy : engineStorage->activeEnemies) {
		double dist = getDistanceTo(enemy);
		if (dist <= storage.getRadius()) {
			if (storage.ownerPlayers.size() > 0) {
				if ((storage.getTargetRelation() == "enemies")) {
					if (enemy->storage.associatedTeam != storage.ownerPlayers[0]->team) {
						result.push_back(enemy);
					}
				}
				else {
					if (enemy->storage.associatedTeam == storage.ownerPlayers[0]->team) {
						result.push_back(enemy);
					}
				}
			}
			else result.push_back(enemy);
		}
	}
	return result;
}

std::vector<std::shared_ptr<TowerActions>> EffectCreatorActions::getTargetTowersInRadius(std::shared_ptr<EngineStorage> engineStorage) {
	std::cout << "[INFO] Trying to find towers for ec : " << storage.getName() << std::endl;
	std::vector<std::shared_ptr<TowerActions>> result;
	if (storage.getRadius() <= 0.0) {
		if (storage.attachedObject->type == MapObjectTypes::Tower) { // Or else nothing happens
			result.push_back(std::static_pointer_cast<TowerActions>(storage.attachedObject));
		}
		return result;
	}

	for (auto& tower : engineStorage->activeTowers) {
		double dist = getDistanceTo(tower);
		if (dist <= storage.getRadius()) {
			if (storage.ownerPlayers.size() > 0) {
				if ((storage.getTargetRelation() == "enemies")) {
					if (tower->storage.ownerPlayers[0]->team != storage.ownerPlayers[0]->team) {
						result.push_back(tower);
					}
				}
				else {
					if (tower->storage.ownerPlayers[0]->team == storage.ownerPlayers[0]->team) {
						result.push_back(tower);
					}
				}
			}
			else result.push_back(tower);
		}
	}
	return result;
}

} // namespace TDEngine::Inner