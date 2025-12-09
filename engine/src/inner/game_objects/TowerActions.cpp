#include "TowerActions.h"

#include "../core/EngineStorage.h"

namespace TDEngine {
    namespace Inner {
        TowerActions::TowerActions(TowerSample sample, std::pair<double, double> startPosition)
            : MapObject(startPosition.first, startPosition.second), storage(std::move(sample)) {
            storage.setUpgradingTo = std::nullopt;
            storage.timeAfterLastShot = UINT64_MAX;
        }

        void TowerActions::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {
            if (storage.setUpgradingTo.has_value()) {
                upgradeTower();
                storage.setUpgradingTo.reset();
            }
            else {
                uint64_t timeBetweenShots = ceil(1000.0 / storage.getFireRate());
                if (storage.timeAfterLastShot < timeBetweenShots) {
                    storage.timeAfterLastShot += timePassedMillis;
                }
                if (storage.timeAfterLastShot >= timeBetweenShots) {
                    auto target = findTarget(engineStorage);
                    if (target != nullptr) {
                        attack(target, engineStorage);
                    }
                }
            }
        }

        void TowerActions::attack(std::shared_ptr<EnemyActions> enemy, std::shared_ptr<EngineStorage> engineStorage) {
            double projectileSpeed = 1; //Вероятно, стоит добавить в параметры башни
            Projectile newProjectile = Projectile(projectileSpeed, storage.getDamage(), enemy, positionCoordinates);
            engineStorage->addProjectile(std::make_shared<Projectile>(newProjectile));
            storage.timeAfterLastShot = 0;
        }

        std::shared_ptr<EnemyActions> TowerActions::findTarget(std::shared_ptr<EngineStorage> engineStorage) {
            std::shared_ptr<EnemyActions> ans = nullptr;
            for (auto enemyPtr : engineStorage->activeEnemies) {
                if (ans == nullptr || getDistanceTo(enemyPtr) < getDistanceTo(ans)) {
                    ans = enemyPtr;
                }
            }
            return ans;
        }

        void TowerActions::setSample(TowerSample sample) {
            storage.setName(sample.getName());
            storage.setDamage(sample.getDamage());
            storage.setFireRate(sample.getFireRate());
            storage.setTowerTexturePath(sample.getTowerTexturePath());
        }

        void TowerActions::upgradeTower() {
            if (storage.setUpgradingTo.has_value()) {
                setSample(storage.setUpgradingTo.value());
            }
        }
    } // Inner
} // TDEngine