#include "EngineStorage.h"

namespace TDEngine {
    namespace Inner {
        //Эту функцию необходимо реализовать с использованием нормального mapSample
        EngineStorage::EngineStorage(std::shared_ptr<Project> project)
            : curGameStatus(GameStatus(0, 0)), curMap(nullptr), curProject(project)
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

        void EngineStorage::cleanMap() {
            for (auto projPtr : activeProjectiles) {
                if (!projPtr->target->storage.isAlive) {
                    removeProjectile(projPtr);
                }
            }
            for (auto enemyPtr : activeEnemies) {
                if (!enemyPtr->storage.isAlive) {
                    removeEnemy(enemyPtr);
                }
            }
        }

        void EngineStorage::addProjectile(const std::shared_ptr<Projectile> &projectile) {
            activeProjectiles.push_back(projectile);
            mapObjects.push_back(projectile);

        }
        void EngineStorage::removeProjectile(const std::shared_ptr<Projectile> &projectile) {
            auto projectileIt =
                std::find(activeProjectiles.begin(), activeProjectiles.end(), projectile);
            if (projectileIt != activeProjectiles.end()) {
                activeProjectiles.erase(projectileIt);
            }

            std::shared_ptr<MapObject> mapObj = std::dynamic_pointer_cast<MapObject>(projectile);
            auto mapIt = std::find(mapObjects.begin(), mapObjects.end(), mapObj);
            if (mapIt != mapObjects.end()) {
                mapObjects.erase(mapIt);
            }

        }

        void EngineStorage::addTower(const std::shared_ptr<TowerActions> &tower) {
            activeTowers.push_back(tower);
            mapObjects.push_back(tower);

        }
        void EngineStorage::removeTower(const std::shared_ptr<TowerActions> &tower) {
            auto towerIt = std::find(activeTowers.begin(), activeTowers.end(), tower);
            if (towerIt != activeTowers.end()) {
                activeTowers.erase(towerIt);
            }

            std::shared_ptr<MapObject> mapObj = std::dynamic_pointer_cast<MapObject>(tower);
            auto mapIt = std::find(mapObjects.begin(), mapObjects.end(), mapObj);
            if (mapIt != mapObjects.end()) {
                mapObjects.erase(mapIt);
            }

        }

        void EngineStorage::addEnemy(const std::shared_ptr<EnemyActions> &enemy) {
            activeEnemies.push_back(enemy);
            mapObjects.push_back(enemy);
        }

        void EngineStorage::removeEnemy(const std::shared_ptr<EnemyActions> &enemy) {
            auto enemyIt = std::find(activeEnemies.begin(), activeEnemies.end(), enemy);
            if (enemyIt != activeEnemies.end()) {
                activeEnemies.erase(enemyIt);
            }

            std::shared_ptr<MapObject> mapObj = std::dynamic_pointer_cast<MapObject>(enemy);
            auto mapIt = std::find(mapObjects.begin(), mapObjects.end(), mapObj);
            if (mapIt != mapObjects.end()) {
                mapObjects.erase(mapIt);
            }
        }
    } // Inner
} // TDEngine