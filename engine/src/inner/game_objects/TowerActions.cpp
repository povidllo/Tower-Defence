#include "TowerActions.h"

#include <utility>

#include "../core/EngineStorage.h"

namespace TDEngine {
	namespace Inner {
		TowerActions::TowerActions(std::shared_ptr<TowerSample> sample, std::pair<double, double> startPosition,
				std::vector<std::shared_ptr<EnginePlayer>> ownerPlayers)
			: MapObject(sample->getTowerTexturePath(), startPosition.first, startPosition.second, MapObjectTypes::Tower),
			storage(*sample) {
			storage.setUpgradingTo = std::nullopt;
			storage.timeAfterLastShot = UINT64_MAX;
			storage.ownerPlayers = std::move(ownerPlayers);
			storage.curFireRate = storage.getFireRate();
			storage.curDamage = storage.getDamage();
			storage.initialActionsDone = false;
			storage.curHp = sample->getStartHP();
			storage.behaviourType = TowerBehaviourTypes::Closest;
			storage.originSample = sample;
			storage.originOwnerPlayers;
			for (auto player : storage.ownerPlayers) {
				storage.originOwnerPlayers.push_back(player);
			}
		}

		TowerActions::TowerActions(std::string texturePath, std::pair<double, double> startPosition,
				std::vector<std::shared_ptr<EnginePlayer>> ownerPlayers, std::vector<std::string> upgrades)
			: MapObject(std::move(texturePath), startPosition.first, startPosition.second, MapObjectTypes::Tower),
				storage(std::move(upgrades)){
			storage.ownerPlayers = std::move(ownerPlayers);
		}

        void TowerActions::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {
			if (storage.curHp <= 0) {
            	std::cout << "[INFO] Tower died: " << storage.getName() << std::endl;
				resetTowerWithSample(storage.originSample, engineStorage);
				storage.ownerPlayers.clear();
				for (auto player : storage.originOwnerPlayers) {
					storage.ownerPlayers.push_back(player);
				}
			}
            if (storage.setUpgradingTo.has_value() && storage.setUpgradingByPlayer != nullptr) {
                upgradeTower(engineStorage);
            }
            else {
            	if (!storage.initialActionsDone) {
            		for (std::string effectCreatorName : storage.getBaseEffectCreatorNames()) {
            			std::cout << "[INFO] Tower spawns ec: " << effectCreatorName << std::endl;
            			auto newEffectCreator = std::make_shared<EffectCreatorActions>(effectCreatorName, engineStorage,
            				storage.ownerPlayers, storage.self);
            			engineStorage->addEffectCreator(newEffectCreator);
            		}
            		storage.initialActionsDone = true;
            	}
            	if (storage.curFireRate > 0) {
            		uint64_t timeBetweenShots = ceil(1000.0 / storage.curFireRate);
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
            Projectile newProjectile = Projectile(storage.getProjectileSpeed(), storage.curDamage, enemy,
            	positionCoordinates, storage.getProjectileTexturePath(), storage.getAttackEffectCreatorNames(),
            	storage.ownerPlayers);
            engineStorage->addProjectile(std::make_shared<Projectile>(newProjectile));
            storage.timeAfterLastShot = 0;
        }

        std::shared_ptr<EnemyActions> TowerActions::findTarget(std::shared_ptr<EngineStorage> engineStorage) {
            std::shared_ptr<EnemyActions> ans = nullptr;
			for (auto enemyPtr : engineStorage->activeEnemies) {
				if (ans == nullptr && getDistanceTo(enemyPtr) <= storage.getFireDistance()) {
					ans = enemyPtr;
				}
            	if (storage.behaviourType == TowerBehaviourTypes::Closest) {
            		if (getDistanceTo(enemyPtr) <= storage.getFireDistance()
            			&& getDistanceTo(enemyPtr) < getDistanceTo(ans)) {
            			ans = enemyPtr;
					}
            	}
            	else if (storage.behaviourType == TowerBehaviourTypes::Farthest) {
            		if (getDistanceTo(enemyPtr) <= storage.getFireDistance()
						&& getDistanceTo(enemyPtr) > getDistanceTo(ans)) {
            			ans = enemyPtr;
					}
            	}
            	else if (storage.behaviourType == TowerBehaviourTypes::LowestHP) {
            		if (getDistanceTo(enemyPtr) <= storage.getFireDistance()
						&& enemyPtr->storage.currentHP < ans->storage.currentHP) {
            			ans = enemyPtr;
					}
            	}
            	else if (storage.behaviourType == TowerBehaviourTypes::HighestHP) {
            		if (getDistanceTo(enemyPtr) <= storage.getFireDistance()
						&& enemyPtr->storage.currentHP > ans->storage.currentHP) {
            			ans = enemyPtr;
					}
            	}
            }
            return ans;
        }

        void TowerActions::resetTowerWithSample(std::shared_ptr<TowerSample> sample, std::shared_ptr<EngineStorage> engineStorage) {
			std::shared_ptr<TowerActions> self = storage.self;
			std::shared_ptr<TowerSample> origin = storage.originSample;
			std::vector<std::shared_ptr<EnginePlayer>> originOwnerPlayers = storage.originOwnerPlayers;
            storage = Tower(*sample);
        	texturePath = sample->getTowerTexturePath();
			storage.curFireRate = storage.getFireRate();
			storage.curDamage = storage.getDamage();
			storage.curHp = sample->getStartHP();
			storage.initialActionsDone = false;
			storage.timeAfterLastShot = 0;
			storage.self = self;
			storage.originSample = origin;
			storage.originOwnerPlayers = originOwnerPlayers;
			for (auto effect : engineStorage->activeTowerEffects) {
				if (effect->storage.target.get() == this) {
					effect->storage.isFinished = true;
				}
			}
			for (auto effectCreator : engineStorage->activeEffectCreators) {
				if (effectCreator->storage.attachedObject.get() == this) {
					effectCreator->storage.isFinished = true;
				}
			}
        }

        void TowerActions::upgradeTower(std::shared_ptr<EngineStorage> engineStorage) {
    		if (!storage.setUpgradingTo.has_value() || storage.setUpgradingByPlayer == nullptr) {
    			return;
    		}

			if (checkOwnership(storage.setUpgradingByPlayer)) {
            	for (const auto& possibleUpgrade : storage.getUpgradeNames()) {
            		if (possibleUpgrade == storage.setUpgradingTo) {
            			for (const auto& sample : engineStorage->curProject->getTowers()) {
            				if (sample->getName() == storage.setUpgradingTo && sample->getCost() <= storage.setUpgradingByPlayer->currentCurrency) {
                				std::cout << "[INFO] Upgrading tower" << std::endl;
            					storage.setUpgradingByPlayer->currentCurrency -= sample->getCost();
            					auto playerT = storage.setUpgradingByPlayer;
            					resetTowerWithSample(sample, engineStorage);
            					storage.ownerPlayers.clear();
            					storage.ownerPlayers.push_back(playerT);
                				std::cout << "[INFO] Tower upgraded to " << storage.getName() << std::endl;
            					return;
            				}
            			}
            		}
            	}
			}
    		storage.setUpgradingTo.reset();
    		storage.setUpgradingByPlayer = nullptr;
        }

    	bool TowerActions::checkOwnership(std::shared_ptr<EnginePlayer> player) {
    		for (auto ownerPlayer : storage.ownerPlayers) {
    			if (ownerPlayer->getPlayerName() == player->getPlayerName()) {
    				std::cout << "[INFO] Tower ownership check: success for player: " << player->getPlayerName() << std::endl;
    				std::cout << "[INFO] Tower ownership check: owner players: ";
    				for (auto printOwner : storage.ownerPlayers) {
    					std::cout << printOwner->getPlayerName() << " ";
    				}
    				std::cout << std::endl;
    				return true;
    			}
    		}
    		std::cout << "[INFO] Tower ownership check: fail for player: " << player->getPlayerName() << std::endl;
    		return false;
    	}
    } // Inner
} // TDEngine