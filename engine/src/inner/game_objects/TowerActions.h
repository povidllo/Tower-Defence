#pragma once

#include "EnemyActions.h"
#include "IActing.h"
#include "MapObject.h"
#include "entities/Tower.h"


namespace TDEngine {
    namespace Inner {
        class TowerActions : public MapObject, public IActing {
        public:
            explicit TowerActions(TowerSample sample, std::pair<double, double> startPosition);
            void act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) override;
            void attack(std::shared_ptr<EnemyActions> enemy, std::shared_ptr<EngineStorage> engineStorage);
            std::shared_ptr<EnemyActions> findTarget(std::shared_ptr<EngineStorage> engineStorage);
            void setSample(TowerSample sample);
            void upgradeTower(std::shared_ptr<EngineStorage> engineStorage);

            Tower storage;
        };
    } // Inner
} // TDEngine