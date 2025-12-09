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

namespace TDEngine {
    namespace Inner {
        class EngineStorage {
            public:
            explicit EngineStorage(std::shared_ptr<Map> map);
            std::vector<std::shared_ptr<IActing>> getEverythingActing();
            std::vector<std::shared_ptr<MapObject>> getAllMapObjects();
            GameStatus getGameStatus();

            //Возможно эти переменные будут приватными и с геттерами-сеттерами
            std::vector<std::shared_ptr<Projectile>> activeProjectiles;
            std::vector<std::shared_ptr<AbilityActions>> activeAbilities;
            std::vector<std::shared_ptr<TowerActions>> activeTowers;
            std::vector<std::shared_ptr<EnemyActions>> activeEnemies;
            std::vector<std::shared_ptr<WaveActions>> activeWaves;
            std::vector<std::shared_ptr<MapObject>> mapObjects;

            private:
            std::shared_ptr<Map> curMap;
            GameStatus curGameStatus;

        };
    } // Inner
} // TDEngine