#pragma once
#include "IActing.h"
#include "entities/Wave.h"

namespace TDEngine {
    namespace Inner {
        class WaveActions : public IActing{
        public:
            explicit WaveActions(WaveSample sample);
            void act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) override;
            void spawnEnemy(std::shared_ptr<EngineStorage> engineStorage);
            void summonNextWave(std::shared_ptr<EngineStorage> engineStorage);

            Wave storage;
        };
    } // Inner
} // TDEngine