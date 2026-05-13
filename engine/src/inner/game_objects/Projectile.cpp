#include "Projectile.h"

#include <utility>

#include "../core/EngineStorage.h"
#include "EffectCreatorActions.h"

namespace TDEngine {
    namespace Inner {
        Projectile::Projectile(double moveSpeed, double damage,
            std::shared_ptr<EnemyActions> target, std::pair<double, double> positionCoordinates,
            std::string texturePath, std::vector<EffectCreatorSample> effectCreatorsOnHit)
                : MapObject(texturePath, positionCoordinates, MapObjectTypes::Projectile),
                    moveSpeed(moveSpeed), damage(damage), target(std::move(target)) {
	        this->isActive = true;
        	this->effectCreatorsOnHit = std::move(effectCreatorsOnHit);
        }

        void Projectile::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {
            moveTo(target->positionCoordinates, moveSpeed, timePassedMillis);
            if (getDistanceTo(target) < 1e-5) {
                hit(engineStorage);
            }
        }

        void Projectile::hit(std::shared_ptr<EngineStorage> engineStorage) {
            target->storage.currentHP -= damage;
        	for (auto &effectSample : effectCreatorsOnHit) {
        		auto newEffectCreator = std::make_shared<EffectCreatorActions>(effectSample, target);
        		engineStorage->addEffectCreator(newEffectCreator);
        	}
        	isActive = false;
        }
    } // Inner
} // TDEngine