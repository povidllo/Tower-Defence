#include "EnemyActions.h"
#include "../core/EngineStorage.h"
namespace TDEngine {
    namespace Inner {
        EnemyActions::EnemyActions(EnemySample sample, std::shared_ptr<Wave> wave)
            : MapObject(sample.getEnemyTexturePath(),
            	wave->getPath()[0].first, wave->getPath()[0].second, MapObjectTypes::Enemy),
    	storage(std::move(sample))
        {

            storage.associatedWave = std::move(wave);
            storage.currentHP = storage.getHealth();
            storage.targetIndex = 0;
            storage.curSpeed = sample.getSpeed();
            storage.isAlive = true;
        	storage.associatedTeam = nullptr;
        }

        void EnemyActions::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {

        	if (storage.isAlive) {
        		if (storage.currentHP <= 0) {
        			killed(engineStorage);
        			return;
        		}

                if (getDistanceTo(storage.associatedWave->getPath()[storage.targetIndex]) < 1e-5) {
                    if (storage.targetIndex + 1 == storage.associatedWave->getPath().size()) {
                        attack(engineStorage);
                    }
                    else {
                        storage.targetIndex++;
                    }
                    }

                moveTo(storage.associatedWave->getPath()[storage.targetIndex],
                	(storage.curSpeed > 0 ? storage.curSpeed : 0), timePassedMillis);
            }
        }


        void EnemyActions::killed(std::shared_ptr<EngineStorage> engineStorage) {
        	std::cout << "[INFO] enemy killed: " << storage.getName() << " by players: " << storage.lastHitPlayers.size() << std::endl;

            storage.isAlive = false;
        	// Инициализация генератора случайных чисел
        	std::srand(static_cast<unsigned int>(std::time(nullptr)));
        	double randomValue = static_cast<double>(std::rand()) / RAND_MAX;
			double finalReward = storage.getMoneyFallsOut() / storage.lastHitPlayers.size();
        	std::cout << "[INFO] final reward " << finalReward << std::endl;
        	if (randomValue < storage.getMoneyFallsOutPercentage() / 100) {
        		for (auto player : storage.lastHitPlayers) {
        			player->currentCurrency += finalReward;
        		}
        	}

        }

        void EnemyActions::attack(std::shared_ptr<EngineStorage> engineStorage) {
        	for (auto player : engineStorage->getAllPlayers()) {
        		player->currentHp -= storage.getDamage();
        		if (player->currentHp <= 0) {
        			player->status = EnginePlayer::LOST;
        		}
        	}
			storage.isAlive = false;
        }

    } // Inner
} // TDEngine