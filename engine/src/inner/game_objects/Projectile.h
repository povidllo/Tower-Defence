#pragma once
#include "EnemyActions.h"
#include "IActing.h"
#include "MapObject.h"

namespace TDEngine {
    namespace Inner {
        class Projectile : public MapObject, public IActing{
        public:
            Projectile(double moveSpeed, double damage,
            std::shared_ptr<EnemyActions> target, std::pair<double, double> positionCoordinates,
            std::string texturePath);
            void act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) override;
            std::shared_ptr<EnemyActions> target;

        private:
            void hit();

            double moveSpeed;
            double damage;
        };
    } // Inner
} // TDEngine