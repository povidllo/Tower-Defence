#pragma once
#include <memory>

#include "../../../../../editor/include/Entity/EnemySample.h"
#include "EngineTeam.h"
#include "Wave.h"
namespace TDEngine {
    namespace Inner {
        class Enemy : public EnemySample{
        public:
            explicit Enemy(EnemySample sample)
                : EnemySample(std::move(sample)){}

            double currentHP;
            std::shared_ptr<Wave> associatedWave;
            int targetIndex;
            double curSpeed;
            bool isAlive;
        	std::shared_ptr<EngineTeam> associatedTeam;
        	std::vector<std::shared_ptr<EnginePlayer>> lastHitPlayers;
        };
    } // Inner
} // TDEngine