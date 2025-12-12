#include "Engine.h"

#include <memory>
namespace TDEngine {
    namespace Inner {
        Engine::Engine(std::shared_ptr<Project> project)
            : storage(std::make_shared<EngineStorage>(project)), //curFrame(std::nullopt),
            boundaryDT(std::make_shared<BoundaryDataTransfer>(this, storage->curProject)),
            tickGen(std::chrono::steady_clock::now())
        {
        }

        void Engine::gameLoop() {
            while (true) {
                while (solveNextAction());
                tickGen.tick(storage);
                storage->cleanMap();
                //ВНИМАНИЕ
                // Сюда можно бы воткнуть функцию boundary-классов на отрисовку
                // auto frame = new FrameData(storage->getGameStatus(), storage->getAllMapObjects());
                // boundaryDT->setNextFrame(*frame);
            }
        }

        void Engine::startGame(const std::string& mapName) {
			bool mapFound = false;
        	for (const auto& map : storage->curProject->getMaps()) {
        		if (map->getName() == mapName) {
        			mapFound = true;
        			storage->curMap = map;
        		}
        	}

        	if (mapFound) {
        		initMap();
        		gameLoop();
        	}
        	else {
        		throw std::invalid_argument("Map not found");
        	}
        }

    	void Engine::initMap() {
        	storage->curWave = 0;
	        storage->activeAbilities.clear();
        	storage->activeEnemies.clear();
        	storage->activeProjectiles.clear();
        	storage->activeTowers.clear();
        	storage->activeWaves.clear();
        	storage->mapObjects.clear();

        	for (const auto& tower : storage->curMap->getSpots()) {
        		storage->addTower(std::make_shared<TowerActions>(
        			TowerActions(*tower, {tower->getX(), tower->getY()})));
        	}
        	storage->addWave(std::make_shared<WaveActions>(WaveActions(*(storage->curMap->getWaves()[0]))));
        }

        void Engine::endGame(bool hasWon) {

        }

        bool Engine::solveNextAction() {
            std::optional<std::shared_ptr<IPlayerAction>> action = boundaryDT->extractPlayerAction();
            if (action.has_value()) {
                action.value()->MakeAction();
                return true;
            }
            return false;
        }





    } // Inner
} // TDEngine