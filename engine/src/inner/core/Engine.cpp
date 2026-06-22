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

        std::shared_ptr<GameStatus> Engine::startGame(const std::string& mapName) {
        	for (const auto& map : storage->curProject->getMaps()) {
        		if (map->getName() == mapName) {
        			storage->curMap = map;
        			initMap();
        			std::cout << "[INFO] Loading complete" << std::endl;
					return storage->curGameStatus;
        		}
        	}
        	throw std::invalid_argument("Map not found");
        }

    	void Engine::initMap() {
	        storage->activeAbilities.clear();
        	storage->activeEnemies.clear();
        	storage->activeProjectiles.clear();
        	storage->activeTowers.clear();
        	storage->activeWaves.clear();
        	storage->activeEnemyEffects.clear();
        	storage->activeTowerEffects.clear();
        	storage->activeEffectCreators.clear();
        	storage->curGameStatus->mapObjects.clear();
        	storage->curGameStatus->teams.clear();

        	storage->reloadMapPlayers();

        	std::cout << "[INFO] Loading towers" << std::endl;
        	for (const auto& tower : storage->curMap->getSpots()) {
        		std::cout << "[INFO] Creating tower for players: " << getAllPlayers().size() << std::endl;
        		storage->addTower(std::make_shared<TowerActions>(
        			TowerActions(tower, {tower->getX(), tower->getY()}, storage->resolveSpotOwnerPlayers(*tower))));
        	}

        	for (auto waveChain : storage->curMap->getStartWaves()) {
        		std::cout << "[INFO] Reading waveChain... " << std::endl;
        		if (waveChain->getChain().size() > 0) {
        			std::cout << "[INFO] Creating wave:" << waveChain->getChain()[0] << std::endl;
        			storage->addWave(std::make_shared<WaveActions>(waveChain->getChain()[0], waveChain, 0, storage));
        		}
        		else {
        			std::cout << "[INFO] Chain is empty! " << std::endl;
        		}
        	}
        }

        void Engine::checkForVictory() {
        	int teamLeft = 0;
        	int teamTotal = 0;
        	for (auto team : storage->curGameStatus->teams) {
        		if (team->teamPlayers.size() > 0) {
        			teamTotal++;
        			if (team->teamPlayers[0]->status == EnginePlayer::PLAYING) {
        				teamLeft++;
        			}
        		}
        	}
			if ((teamLeft == 1 && teamTotal > 1) || (storage->activeWaves.size() == 0 &&
				storage->activeEnemies.size() == 0)) {
				for (auto player : getAllPlayers()) {
					if (player->status == EnginePlayer::PLAYING) {
						player->status = EnginePlayer::WON;
					}
				}
			}
        }

    	std::vector<std::shared_ptr<EnginePlayer>> Engine::getAllPlayers() {
        	return storage->getAllPlayers();
		}





    } // Inner
} // TDEngine
