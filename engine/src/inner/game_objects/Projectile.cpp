#include "Projectile.h"

#include <utility>

#include "../core/EngineStorage.h"
#include "EffectCreatorActions.h"

namespace TDEngine {
    namespace Inner {
        Projectile::Projectile(double moveSpeed, double damage,
            std::shared_ptr<EnemyActions> target, std::pair<double, double> positionCoordinates,
            std::string texturePath, std::vector<std::string> effectCreatorsOnHit,
				std::vector<std::shared_ptr<EnginePlayer>> ownerPlayers)
                : MapObject(texturePath, positionCoordinates, MapObjectTypes::Projectile),
                    moveSpeed(moveSpeed), damage(damage), target(std::move(target)) {
	        this->isActive = true;
        	this->effectCreatorsOnHitNames = std::move(effectCreatorsOnHit);
        	this->ownerPlayers = std::move(ownerPlayers);
        }

        void Projectile::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {
            moveTo(target->positionCoordinates, moveSpeed, timePassedMillis);
            if (getDistanceTo(target) < 1e-5) {
                hit(engineStorage);
            }
        }

        void Projectile::hit(std::shared_ptr<EngineStorage> engineStorage) {
            target->storage.currentHP -= damage;
        	target->storage.lastHitPlayers = std::move(ownerPlayers);
        	for (std::string effectCreatorName : effectCreatorsOnHitNames) {
        		auto newEffectCreator = std::make_shared<EffectCreatorActions>(effectCreatorName, engineStorage, ownerPlayers, target);
        		engineStorage->addEffectCreator(newEffectCreator);
        	}

        	for (std::string effectCreatorName : target->storage.getDamageTakenEffectCreatorNames()) {
        		auto newEffectCreator = std::make_shared<EffectCreatorActions>(effectCreatorName, engineStorage,
					ownerPlayers, std::make_shared<EnemyActions>(*target));
        		engineStorage->addEffectCreator(newEffectCreator);
        	}
        	isActive = false;
        }
    } // Inner
} // TDEngine