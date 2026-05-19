#include "EffectCreatorActions.h"
#include "EnemyActions.h"
#include "TowerActions.h"
#include "EffectOnEnemyActions.h"
#include "EffectOnTowerActions.h"
#include "../core/EngineStorage.h"

namespace TDEngine::Inner {

EffectCreatorActions::EffectCreatorActions(EffectCreatorSample sample, std::shared_ptr<MapObject> attachedObject)
    : MapObject("", attachedObject->positionCoordinates, MapObjectTypes::EffectCreator), // type placeholder
      storage(std::move(sample)) {
	storage.isFinished = false;
	storage.elapsedTime = 0.0;
	storage.timeSinceLastPeriod = 0.0;
	if (storage.attachment == EffectAttachment::Point) {
		storage.attachedObject = std::make_shared<MapObject>
			(MapObject("", attachedObject->positionCoordinates, MapObjectTypes::Point));
	}
	else {
		storage.attachedObject = attachedObject;
	}
}

void EffectCreatorActions::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {
    if (storage.isFinished) return;

    double dt = timePassedMillis / 1000.0;
    storage.elapsedTime += dt;

    // If attached to an object that died, destroy self
    if (storage.attachment == EffectAttachment::MapObject && storage.attachedObject->type == MapObjectTypes::Enemy) {
    	auto enemy = std::static_pointer_cast<EnemyActions>(storage.attachedObject);
    	if (!enemy->storage.isAlive) {
    		storage.isFinished = true;
    		return;
    	}
    }

    // Update position if attached to moving object
    if (storage.attachment == EffectAttachment::MapObject) {
        positionCoordinates = storage.attachedObject->positionCoordinates;
    }

    // Apply initial effects once
    if (storage.elapsedTime - dt <= 0.0) {
        applyEffects(storage.initialEnemyEffects, engineStorage);
    }

    // Periodic application
    if (storage.period > 0.0) {
        storage.timeSinceLastPeriod += dt;
        while (storage.timeSinceLastPeriod >= storage.period) {
            applyEffects(storage.periodicEnemyEffects, engineStorage);
            storage.timeSinceLastPeriod -= storage.period;
        }
    }

    if (storage.elapsedTime >= storage.duration) {
        storage.isFinished = true;
    }
}


	//for now wothout tower effects
void EffectCreatorActions::applyEffects(const std::vector<EffectOnEnemySample>& enemyEffects,
                                        std::shared_ptr<EngineStorage> engineStorage) {
    auto enemies = getTargetEnemiesInRadius(engineStorage);
    for (auto& target : enemies) {
        for (const auto& effectSample : enemyEffects) {
            auto newEffect = std::make_shared<EffectOnEnemyActions>(effectSample, target);
            engineStorage->addEffectOnEnemy(newEffect);
        }
    }
}

std::vector<std::shared_ptr<EnemyActions>> EffectCreatorActions::getTargetEnemiesInRadius(std::shared_ptr<EngineStorage> engineStorage) {
    std::vector<std::shared_ptr<EnemyActions>> result;
    if (storage.radius <= 0.0) {
    	if (storage.attachment == EffectAttachment::MapObject && storage.attachedObject->type == MapObjectTypes::Enemy) { // Or else nothing happens
    		result.push_back(std::static_pointer_cast<EnemyActions>(storage.attachedObject));
    	}
        return result;
    }

    for (auto& enemy : engineStorage->activeEnemies) {
        double dist = getDistanceTo(enemy);
        if (storage.radius > 0.0 && dist > storage.radius) continue;

        result.push_back(enemy);
    }
    return result;
}

} // namespace TDEngine::Inner