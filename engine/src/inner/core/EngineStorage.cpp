#include "EngineStorage.h"

namespace TDEngine {
    namespace Inner {
        //Эту функцию необходимо реализовать с использованием нормального mapSample
        EngineStorage::EngineStorage(std::shared_ptr<Map> map)
            : curMap(map), curGameStatus(GameStatus(0, 0))
        {
        }

        std::vector<std::shared_ptr<IActing>> EngineStorage::getEverythingActing() {
            std::vector<std::shared_ptr<IActing>> actings;

            actings.insert(actings.end(), activeProjectiles.begin(), activeProjectiles.end());
            actings.insert(actings.end(), activeAbilities.begin(), activeAbilities.end());
            actings.insert(actings.end(), activeTowers.begin(), activeTowers.end());
            actings.insert(actings.end(), activeEnemies.begin(), activeEnemies.end());
            actings.insert(actings.end(), activeWaves.begin(), activeWaves.end());

            return actings;
        }

        std::vector<std::shared_ptr<MapObject>> EngineStorage::getAllMapObjects() {
            return mapObjects;
        }

        GameStatus EngineStorage::getGameStatus() {
            return curGameStatus;
        }


    } // Inner
} // TDEngine