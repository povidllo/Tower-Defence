#pragma once
#include <memory>

#include "../../../../../editor/include/Entity/EnemySample.h"
#include "EngineTeam.h"
#include "Wave.h"
namespace TDEngine {
    namespace Inner {
		class EnemyActions;
		class Enemy : public EnemySample{
        public:
        	explicit Enemy(EnemySample sample)
        	: EnemySample(std::move(sample)){}
        	explicit Enemy()
				: EnemySample(std::string("")){}

            double currentHP;
            std::shared_ptr<Wave> associatedWave;
            int targetIndex;
            double curSpeed;
            bool isAlive;
        	bool initialActionsDone;
        	std::shared_ptr<EngineTeam> associatedTeam;
        	std::vector<std::shared_ptr<EnginePlayer>> lastHitPlayers;
        	std::shared_ptr<EnemyActions> self;
        };
    } // Inner
} // TDEngine