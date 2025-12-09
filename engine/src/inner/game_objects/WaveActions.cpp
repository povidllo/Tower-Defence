#include "WaveActions.h"

#include "EnemyActions.h"
#include "../core/EngineStorage.h"

namespace TDEngine {
    namespace Inner {
        WaveActions::WaveActions(WaveSample sample)
            : storage(std::move(sample)) {
            storage.enemiesSpawned = 0;
            storage.spawningIndex = 0;
            storage.timeAfterLastSpawn = 0;
        }

        void WaveActions::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {
            uint64_t spawnInterval = 500;
            if (storage.timeAfterLastSpawn < spawnInterval) {
                storage.timeAfterLastSpawn += timePassedMillis;
            }
            if (storage.timeAfterLastSpawn >= spawnInterval && storage.spawningIndex < storage.getEnemies().size()) {
                if (storage.spawningIndex == storage.getEnemies().size() - 1 &&
                    storage.enemiesSpawned == storage.getEnemies()[storage.spawningIndex].second) {
                    summonNextWave(engineStorage);
                    return;
                }

                if (storage.enemiesSpawned == storage.getEnemies()[storage.spawningIndex].second) {
                    storage.spawningIndex++;
                    storage.enemiesSpawned = 0;
                }

                spawnEnemy(engineStorage);
            }
        }

        void WaveActions::spawnEnemy(std::shared_ptr<EngineStorage> engineStorage) {
            std::string enemyName = storage.getEnemies()[storage.spawningIndex].first;
            for (auto enPtr : engineStorage->curProject->getEnemies()) {
                if (enPtr->getName() == enemyName) {
                    EnemySample enemySample = EnemySample(*enPtr);
                    EnemyActions enemy = EnemyActions(enemySample, std::make_shared<Wave>(storage));
                    engineStorage->addEnemy(std::make_shared<EnemyActions>(enemy));
                    storage.spawningIndex++;
                    storage.timeAfterLastSpawn = 0;
                    break;
                }
            }
        }

        void WaveActions::summonNextWave(std::shared_ptr<EngineStorage> engineStorage) {
            //Сюда надо добавить спавн следующей волны, но пока это, вроде, невозможно
            //Нужно обновить энтити в редакторе
            storage.spawningIndex++;
        }




    } // Inner
} // TDEngine