#pragma once
#include <memory>

#include "Wave.h"
#include "../../../../../editor/include/Entity/EnemySample.h"
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
        };
    } // Inner
} // TDEngine