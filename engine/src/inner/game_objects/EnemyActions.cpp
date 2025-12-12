#include "EnemyActions.h"
#include "../core/EngineStorage.h"
namespace TDEngine {
    namespace Inner {
        EnemyActions::EnemyActions(EnemySample sample, std::shared_ptr<Wave> wave)
            : MapObject(wave->getPath()[0].first, wave->getPath()[0].second), storage(std::move(sample))
        {

            storage.associatedWave = std::move(wave);
            storage.currentHP = storage.getHealth();
            storage.targetIndex = 0;
            storage.curSpeed = 1;
            storage.isAlive = true;
        }

        void EnemyActions::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {
            if (storage.currentHP <= 0) {
                storage.isAlive = false;
            }

            if (storage.isAlive) {
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


        void EnemyActions::die() {
            storage.isAlive = false;
        }

        void EnemyActions::attack(std::shared_ptr<EngineStorage> engineStorage) {
            engineStorage->curGameStatus.currentHp -= storage.getDamage();
        	if (engineStorage->curGameStatus.currentHp <= 0) {
        		engineStorage->isPlaying = false;
        	}
        }


    } // Inner
} // TDEngine