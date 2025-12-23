#include "Engine.h"

#include <memory>
namespace TDEngine {
    namespace Inner {
        Engine::Engine(std::shared_ptr<Project> project)
            : storage(std::make_shared<EngineStorage>(project)),
            // boundaryDT(std::make_shared<BoundaryDataTransfer>(this, storage->curProject)),
            tickGen(std::chrono::steady_clock::now())
        {
        }

        std::shared_ptr<GameStatus> Engine::gameStep(std::shared_ptr<IPlayerAction> action) {
                if (action != nullptr) {
	                action->MakeAction();
                }
                tickGen.tick(storage);
                storage->cleanMap();
        		checkForVictory();
				return storage->curGameStatus;
        }

        std::shared_ptr<GameStatus> Engine::startGame(const std::string& mapName) {
        	for (const auto& map : storage->curProject->getMaps()) {
        		if (map->getName() == mapName) {
        			storage->curMap = map;
        			initMap();
					return storage->curGameStatus;
        		}
        	}
        	throw std::invalid_argument("Map not found");
        }

    	void Engine::initMap() {
        	storage->curWave = 0;
	        storage->activeAbilities.clear();
        	storage->activeEnemies.clear();
        	storage->activeProjectiles.clear();
        	storage->activeTowers.clear();
        	storage->activeWaves.clear();
        	storage->curGameStatus->mapObjects.clear();
			storage->curGameStatus->currentGold = storage->curMap->getStartCurrency();
        	storage->curGameStatus->currentHp = storage->curMap->getHp();
        	storage->curGameStatus->status = GameStatus::PLAYING;

        	for (const auto& tower : storage->curMap->getSpots()) {
        		storage->addTower(std::make_shared<TowerActions>(
        			TowerActions(*tower, {tower->getX(), tower->getY()})));
        	}
        	storage->addWave(std::make_shared<WaveActions>(WaveActions(*(storage->curMap->getWaves()[0]))));
        }

        void Engine::checkForVictory() {
			if (storage->activeWaves.size() == 0 &&
				storage->activeEnemies.size() == 0) {
				storage->curGameStatus->status = GameStatus::WON;
			}
        }





    } // Inner
} // TDEngine