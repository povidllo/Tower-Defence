#pragma once
#include "IActing.h"
#include "MapObject.h"
#include "entities/Enemy.h"
#include "../../../../editor/include/Entity/EnemySample.h"

namespace TDEngine {
    namespace Inner {
        class EngineStorage;
        class EnemyActions : public MapObject, public  IActing {
        public:
            explicit EnemyActions(EnemySample sample, std::shared_ptr<Wave> wave);
            void act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) override;
            void move(uint64_t timePassedMillis);
            void attack(std::shared_ptr<EngineStorage> engineStorage);
            void die();


            Enemy storage;
        };
    } // Inner
} // TDEngine