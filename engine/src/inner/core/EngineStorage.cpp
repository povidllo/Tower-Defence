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

        // void EngineStorage::cleanMap() {
        //
        // }

        void EngineStorage::addEnemy(std::shared_ptr<EnemyActions> enemy) {
            activeEnemies.push_back(enemy);
            mapObjects.push_back(enemy);
        }

        void EngineStorage::removeEnemy(std::shared_ptr<EnemyActions> enemy) {
            auto enemyIt = std::find(activeEnemies.begin(), activeEnemies.end(), enemy);
            if (enemyIt != activeEnemies.end()) {
                activeEnemies.erase(enemyIt);
            }

            std::shared_ptr<MapObject> mapObj = std::dynamic_pointer_cast<MapObject>(enemy);
            if (mapObj) {
                auto mapIt = std::find(mapObjects.begin(), mapObjects.end(), mapObj);
                if (mapIt != mapObjects.end()) {
                    mapObjects.erase(mapIt);
                }
            }
        }




    } // Inner
} // TDEngine