#include "Engine.h"

#include <memory>
namespace TDEngine {
    namespace Inner {
        Engine::Engine(std::shared_ptr<Project> project)
            : storage(std::make_shared<EngineStorage>(project)),
            boundaryDT(std::make_shared<BoundaryDataTransfer>(this, storage->curProject)),
            tickGen(std::chrono::steady_clock::now())
        {
        }

        std::shared_ptr<GameStatus> Engine::gameStep() {
                // while (solveNextAction());
                tickGen.tick(storage);
                storage->cleanMap();
				return storage->curGameStatus;
        }

        std::shared_ptr<GameStatus> Engine::startGame(const std::string& mapName) {
			bool mapFound = false;
        	for (const auto& map : storage->curProject->getMaps()) {
        		if (map->getName() == mapName) {
        			mapFound = true;
        			storage->curMap = map;
        		}
        	}

        	if (mapFound) {
        		initMap();
				return storage->curGameStatus;
        	}
        	else {
        		throw std::invalid_argument("Map not found");
        	}
        }

    	void Engine::initMap() {
        	storage->curWave = 0;
        	storage->isPlaying = true;
	        storage->activeAbilities.clear();
        	storage->activeEnemies.clear();
        	storage->activeProjectiles.clear();
        	storage->activeTowers.clear();
        	storage->activeWaves.clear();
        	storage->curGameStatus->mapObjects.clear();
			storage->curGameStatus->currentGold = storage->curMap->getStartCurrency();
        	storage->curGameStatus->currentHp = storage->curMap->getHp();

        	for (const auto& tower : storage->curMap->getSpots()) {
        		storage->addTower(std::make_shared<TowerActions>(
        			TowerActions(*tower, {tower->getX(), tower->getY()})));
        	}
        	storage->addWave(std::make_shared<WaveActions>(WaveActions(*(storage->curMap->getWaves()[0]))));
        }

        void Engine::endGame() {
        }

        bool Engine::solveNextAction() {
            // std::optional<std::shared_ptr<IPlayerAction>> action = boundaryDT->extractPlayerAction();
            // if (action.has_value()) {
            //     action.value()->MakeAction();
            //     return true;
            // }
            return false;
        }

    	bool Engine::isPlaying() {
	        return storage->isPlaying;
        }





    } // Inner
} // TDEngine