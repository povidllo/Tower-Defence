#include "TowerActions.h"

#include <utility>

#include "../core/EngineStorage.h"

namespace TDEngine {
    namespace Inner {
    	TowerActions::TowerActions(TowerSample sample, std::pair<double, double> startPosition,
				std::vector<std::shared_ptr<EnginePlayer>> ownerPlayers)
            : MapObject(sample.getTowerTexturePath(), startPosition.first, startPosition.second, MapObjectTypes::Tower),
    		storage(std::move(sample)) {
            storage.setUpgradingTo = std::nullopt;//storage.getUpgradeNames()[0];//
            storage.timeAfterLastShot = UINT64_MAX;
    		storage.ownerPlayers = std::move(ownerPlayers);

        	// storage.effectCreatorsOnHit.push_back(createTestEffectCreatorSample());
        }

        void TowerActions::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {
            if (storage.setUpgradingTo.has_value()) {
                upgradeTower(engineStorage);
                storage.setUpgradingTo.reset();
            }
            else {
            	if (storage.getFireRate() > 0) {
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
        }

        void TowerActions::attack(std::shared_ptr<EnemyActions> enemy, std::shared_ptr<EngineStorage> engineStorage) {
            Projectile newProjectile = Projectile(storage.getProjectileSpeed(), storage.getDamage(), enemy,
            	positionCoordinates, storage.getProjectileTexturePath(), storage.effectCreatorsOnHit,
            	storage.ownerPlayers);
            engineStorage->addProjectile(std::make_shared<Projectile>(newProjectile));
            storage.timeAfterLastShot = 0;
        }

        std::shared_ptr<EnemyActions> TowerActions::findTarget(std::shared_ptr<EngineStorage> engineStorage) {
            std::shared_ptr<EnemyActions> ans = nullptr;
            for (auto enemyPtr : engineStorage->activeEnemies) {
                if ((ans == nullptr || getDistanceTo(enemyPtr) < getDistanceTo(ans))
                	&& getDistanceTo(enemyPtr) <= storage.getFireDistance()) {
                    ans = enemyPtr;
                }
            }
            return ans;
        }

        void TowerActions::setSample(std::shared_ptr<TowerSample> sample) {
            storage = Tower(*sample);
        	texturePath = sample->getTowerTexturePath();
        	// storage.effectCreatorsOnHit.push_back(createTestEffectCreatorSample());
        }

        void TowerActions::upgradeTower(std::shared_ptr<EngineStorage> engineStorage) {
    		if (!storage.setUpgradingTo.has_value() || !storage.setUpgradingByPlayer.has_value()) {
    			return;
    		}
    		bool playerIsOwner = false;
    		std:
    		for (const auto& owner : storage.ownerPlayers) {
    			if (owner == storage.setUpgradingByPlayer) {
    				playerIsOwner = true;
    			}
    		}
            if (playerIsOwner) {
            	for (const auto& possibleUpgrade : storage.getUpgradeNames()) {
            		if (possibleUpgrade == storage.setUpgradingTo) {
            			for (const auto& sample : engineStorage->curProject->getTowers()) {
            				if (sample->getName() == storage.setUpgradingTo && sample->getCost() <= storage.setUpgradingByPlayer.value()->currentCurrency) {
            					storage.setUpgradingByPlayer.value()->currentCurrency -= sample->getCost();
            					setSample(sample);
            					storage.ownerPlayers.clear();
            					storage.ownerPlayers.push_back(storage.setUpgradingByPlayer.value());
            					storage.timeAfterLastShot = 0;
            					storage.setUpgradingTo = std::nullopt;
            					storage.setUpgradingByPlayer = std::nullopt;
            					return;
            				}
            			}
            		}
            	}
            }
        }

    	EffectCreatorSample TowerActions::createTestEffectCreatorSample() {

        	EffectOnEnemySample effectSample;
        	effectSample.duration = 3;
        	effectSample.period = 0.5;
        	effectSample.canStack = true;
        	effectSample.periodicHealth = -10;
        	effectSample.initialHealth = 0;
        	effectSample.initialSpeedPercent = 20;
        	effectSample.periodicSpeedPercent = 0;

        	EffectCreatorSample effectCreator;
        	effectCreator.initialEnemyEffects.push_back(effectSample);
        	effectCreator.radius = 0;
        	effectCreator.duration = 0;
        	effectCreator.period = 0;
        	effectCreator.attachment = EffectAttachment::MapObject;
        	effectCreator.targetType = EffectTargetType::Enemies;
        	return  effectCreator;
        }
    } // Inner
} // TDEngine