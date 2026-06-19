#pragma once
#include "../../../../editor/include/Entity/EnemySample.h"
#include "IActing.h"
#include "MapObject.h"
#include "entities/Enemy.h"

namespace TDEngine {
    namespace Inner {
        class EngineStorage;
        class EnemyActions : public MapObject, public  IActing {
        public:
            explicit EnemyActions(EnemySample sample, std::shared_ptr<Wave> wave, std::shared_ptr<EngineTeam> team);
            explicit EnemyActions(std::string texturePath, std::pair<double, double> startPosition,
            	double currentHp, double maxHp);
            void act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) override;
            void attack(std::shared_ptr<EngineStorage> engineStorage);
        	void killed(std::shared_ptr<EngineStorage> engineStorage);

            Enemy storage;
        };
    } // Inner
} // TDEngine