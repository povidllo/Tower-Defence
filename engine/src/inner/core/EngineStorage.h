#pragma once
#include <memory>
#include <vector>

#include "../game_entities/AbilityActions.h"
#include "../game_entities/EnemyActions.h"
#include "../game_entities/GameStatus.h"
#include "../game_entities/IActing.h"
#include "../game_entities/MapObject.h"
#include "../game_entities/Projectile.h"
#include "../game_entities/TowerActions.h"
#include "../game_entities/WaveActions.h"
#include "../Zaglushki/MapSample.h"

namespace TDEngine {
    namespace Inner {
        class EngineStorage {
            public:
            explicit EngineStorage(MapSample map);
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
            MapSample curMap;
            GameStatus curGameStatus;

        };
    } // Inner
} // TDEngine