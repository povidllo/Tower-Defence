#include "Engine.h"

#include <memory>
namespace TDEngine {
    namespace Inner {
        Engine::Engine(std::shared_ptr<Project> project)
            : storage(std::make_shared<EngineStorage>(project)),
            tickGen(std::chrono::steady_clock::now())
        {
        }

        std::shared_ptr<GameStatus> Engine::gameStep(std::shared_ptr<IPlayerAction> action) {
        		// std::cout << "[INFO] Game step started" << std::endl;
                if (action != nullptr) {
                	std::cout << "[INFO] Engine makes action" << std::endl;
	                action->MakeAction();
                }
                tickGen.tick(storage);
                storage->cleanMap();
        		checkForVictory();
				return storage->curGameStatus;
        		// std::cout << "[INFO] Game step ended" << std::endl;
        }

        std::shared_ptr<GameStatus> Engine::startGame(const std::string& mapName, bool cooperativePlay) {
        	for (const auto& map : storage->curProject->getMaps()) {
        		if (map->getName() == mapName) {
        			storage->curMap = map;
        			storage->setCooperativePlay(cooperativePlay);
        			initMap();
        			std::cout << "[INFO] Loading complete" << std::endl;
					return storage->curGameStatus;
        		}
        	}
        	throw std::invalid_argument("Map not found");
        }

    	void Engine::initMap() {
        	storage->curWave = 0;
	        // storage->activeAbilities.clear();
        	storage->activeEnemies.clear();
        	storage->activeProjectiles.clear();
        	storage->activeTowers.clear();
        	storage->activeWaves.clear();
        	storage->activeEnemyEffects.clear();
        	storage->activeEffectCreators.clear();
        	storage->curGameStatus->mapObjects.clear();
        	storage->curGameStatus->teams.clear();

        	storage->reloadMapPlayers();

        	std::cout << "[INFO] Loading towers" << std::endl;
        	for (const auto& spot : storage->curMap->getSpots()) {
        		const auto owners = storage->resolveSpotOwnerPlayers(*spot);
        		std::cout << "[INFO] Creating tower at (" << spot->getX() << ", " << spot->getY()
        		          << ") for " << owners.size() << " owner(s)" << std::endl;
        		storage->addTower(std::make_shared<TowerActions>(
        			TowerActions(*spot, {spot->getX(), spot->getY()}, owners)));
        	}

        	if (!storage->curMap->getWaves().empty()) {
        		storage->addWave(std::make_shared<WaveActions>(WaveActions(*(storage->curMap->getWaves()[0]))));
        	}
        }

        void Engine::checkForVictory() {
			if (storage->activeWaves.size() == 0 &&
				storage->activeEnemies.size() == 0) {
				for (auto player : getAllPlayers()) {
					player->status = EnginePlayer::WON;
				}
			}
        }

    	std::vector<std::shared_ptr<EnginePlayer>> Engine::getAllPlayers() {
        	return storage->getAllPlayers();
		}





    } // Inner
} // TDEngine
