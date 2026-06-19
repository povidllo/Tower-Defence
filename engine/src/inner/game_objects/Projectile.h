#pragma once
#include "EnemyActions.h"
#include "IActing.h"
#include "MapObject.h"
#include "../../../editor/include/Entity/EffectCreatorSample.h"

namespace TDEngine {
    namespace Inner {
        class Projectile : public MapObject, public IActing{
        public:
            Projectile(double moveSpeed, double damage,
            std::shared_ptr<EnemyActions> target, std::pair<double, double> positionCoordinates,
            std::string texturePath, std::vector<std::string> effectCreatorsOnHit,
				std::vector<std::shared_ptr<EnginePlayer>> ownerPlayers);
            void act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) override;
            std::shared_ptr<EnemyActions> target;


        	bool isActive;
        private:
            void hit(std::shared_ptr<EngineStorage> engineStorage);

            double moveSpeed;
            double damage;
        	std::vector<std::string> effectCreatorsOnHitNames;
        	std::vector<std::shared_ptr<EnginePlayer>> ownerPlayers;
        };
    } // Inner
} // TDEngine