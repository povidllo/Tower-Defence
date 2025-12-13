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
        }

        void EnemyActions::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {

        	if (storage.isAlive) {
        		if (storage.currentHP <= 0) {
        			die(engineStorage);
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

                moveTo(storage.associatedWave->getPath()[storage.targetIndex], storage.curSpeed, timePassedMillis);
            }
        }


        void EnemyActions::die(std::shared_ptr<EngineStorage> engineStorage) {
            storage.isAlive = false;

        	// Инициализация генератора случайных чисел
        	std::srand(static_cast<unsigned int>(std::time(nullptr)));
        	double randomValue = static_cast<double>(std::rand()) / RAND_MAX;

        	if (randomValue < storage.getMoneyFallsOutPercentage() / 100) {
        		engineStorage->curGameStatus->currentGold += storage.getMoneyFallsOut();
        	}

        }

        void EnemyActions::attack(std::shared_ptr<EngineStorage> engineStorage) {
            engineStorage->curGameStatus->currentHp -= storage.getDamage();
        	if (engineStorage->curGameStatus->currentHp <= 0) {
        		engineStorage->isPlaying = false;
        	}
        }


    } // Inner
} // TDEngine