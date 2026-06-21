#include "WaveActions.h"

#include <utility>

#include "EnemyActions.h"
#include "../core/EngineStorage.h"

namespace TDEngine {
    namespace Inner {
        WaveActions::WaveActions(std::string sampleName, std::shared_ptr<WaveChain> chain, int chainIndex, std::shared_ptr<EngineStorage> engineStorage)
            : storage(*findSample(sampleName, engineStorage)) {
            storage.enemiesSpawned = 0;
            storage.spawningIndex = 0;
            storage.timeAfterLastSpawn = 0;
            storage.delayTime = ceil(chain->getDelays()[chainIndex] * 1000);
        	storage.chain = std::move(chain);
        	storage.chainIndex = chainIndex;
        }

        void WaveActions::act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) {
        	if (storage.spawningIndex == storage.getEnemies().size() + 1) return;
        	if (storage.delayTime > timePassedMillis) {
        		storage.delayTime -= timePassedMillis;
        		return;
        	}
        	else {
        		storage.delayTime = 0;
        	}

        	if (storage.spawningIndex == storage.getEnemies().size()) {
        		summonNextWave(engineStorage);
        		return;
			}
            if (storage.timeAfterLastSpawn < storage.getEnemySpawnInterval() * 1000) {
                storage.timeAfterLastSpawn += timePassedMillis;
            }
            if (storage.timeAfterLastSpawn >= storage.getEnemySpawnInterval() * 1000 && storage.spawningIndex < storage.getEnemies().size()) {
                spawnEnemy(engineStorage);
                if (storage.enemiesSpawned == storage.getEnemies()[storage.spawningIndex].second) {
                    storage.spawningIndex++;
                    storage.enemiesSpawned = 0;
                }
            }
        }

        void WaveActions::spawnEnemy(std::shared_ptr<EngineStorage> engineStorage) {
            std::string enemyName = storage.getEnemies()[storage.spawningIndex].first;
        	std::cout << "[INFO] Spawning enemy... " << enemyName << std::endl;
            for (auto enPtr : engineStorage->curProject->getEnemies()) {
                if (enPtr->getName() == enemyName) {
                    EnemySample enemySample = EnemySample(*enPtr);
                	std::shared_ptr<EngineTeam> belongs = nullptr;
                	for (auto team : engineStorage->curGameStatus->teams) {
                		for (auto player : team->teamPlayers) {
                			if (storage.getBelongs().size() > 0 && player->getPlayerName() == storage.getBelongs()[0]) {
                				belongs = team;
                				break;
                			}
                		}
                		if (belongs != nullptr) break;
                	}
                    std::shared_ptr<EnemyActions> enemy = std::make_shared<EnemyActions>(enemySample, std::make_shared<Wave>(storage), belongs);
                    engineStorage->addEnemy(enemy);

                	std::vector<std::shared_ptr<EnginePlayer>> emptyVector;
                	for (std::string effectCreatorName : enemy->storage.getBaseEffectCreatorNames()) {
                		auto newEffectCreator = std::make_shared<EffectCreatorActions>(effectCreatorName, engineStorage,
								(enemy->storage.associatedTeam != nullptr? enemy->storage.associatedTeam->teamPlayers : emptyVector),
								enemy);
                		engineStorage->addEffectCreator(newEffectCreator);
                	}

                    storage.enemiesSpawned++;
                    storage.timeAfterLastSpawn = 0;
                    return;;
                }
            }
        	std::cout << "[INFO] Enemy not found! " << enemyName << std::endl;
        }

        void WaveActions::summonNextWave(std::shared_ptr<EngineStorage> engineStorage) {
        	std::cout << "[INFO] Summoning next wave (index, size): " << storage.chainIndex << " of " << storage.chain->getChain().size()  << std::endl;
        	if (storage.chainIndex + 1 < storage.chain->getChain().size()) {
        		engineStorage->addWave(std::make_shared<WaveActions>
        			(storage.chain->getChain()[storage.chainIndex+1], storage.chain, storage.chainIndex+1, engineStorage));
        	}
            storage.spawningIndex++;
        }

    	std::shared_ptr<WaveSample> WaveActions::findSample(std::string sampleName, std::shared_ptr<EngineStorage> engineStorage) {
        	for (auto waveSample: engineStorage->curMap->getWaves()) {
        		if (waveSample->getName() == sampleName) {
        			return waveSample;
        		}
        	}
        	std::cout << "[INFO] Wave sample not found:" << sampleName << std::endl;
        	return nullptr;
        }




    } // Inner
} // TDEngine