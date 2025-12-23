#pragma once
#include <memory>
#include <vector>

#include "../game_objects/AbilityActions.h"
#include "../game_objects/EnemyActions.h"
#include "../game_objects/GameStatus.h"
#include "../game_objects/IActing.h"
#include "../game_objects/MapObject.h"
#include "../game_objects/Projectile.h"
#include "../game_objects/TowerActions.h"
#include "../game_objects/WaveActions.h"
#include "../../../../editor/include/Entity/Map.h"
#include "../../../../editor/include/Entity/Project.h"

namespace TDEngine {
    namespace Inner {
        class EngineStorage {
            public:
            explicit EngineStorage(std::shared_ptr<Project> project);
            std::vector<std::shared_ptr<IActing>> getEverythingActing();
            // std::vector<std::shared_ptr<MapObject>> getAllMapObjects();

            void cleanMap();
            void addProjectile(const std::shared_ptr<Projectile> &projectile);
            void removeProjectile(const std::shared_ptr<Projectile> &projectile);
            void addTower(const std::shared_ptr<TowerActions> &tower);
        	void removeTower(const std::shared_ptr<TowerActions> &tower);
        	void addEnemy(const std::shared_ptr<EnemyActions> &enemy);
        	void removeEnemy(const std::shared_ptr<EnemyActions> &enemy);
        	void addWave(const std::shared_ptr<WaveActions> &wave);
        	void removeWave(const std::shared_ptr<WaveActions> &wave);


            std::vector<std::shared_ptr<Projectile>> activeProjectiles;
            std::vector<std::shared_ptr<AbilityActions>> activeAbilities;
            std::vector<std::shared_ptr<TowerActions>> activeTowers;
            std::vector<std::shared_ptr<EnemyActions>> activeEnemies;
            std::vector<std::shared_ptr<WaveActions>> activeWaves;
            std::shared_ptr<GameStatus> curGameStatus;
            std::shared_ptr<Project> curProject;

            std::shared_ptr<Map> curMap;
        	int curWave;

        };
    } // Inner
} // TDEngine