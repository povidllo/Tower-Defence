#pragma once
#include "IActing.h"
#include "entities/Wave.h"

namespace TDEngine {
    namespace Inner {
        class WaveActions : public IActing{
        public:
            explicit WaveActions(std::string sampleName, std::shared_ptr<WaveChain> chain, int chainIndex, std::shared_ptr<EngineStorage> engineStorage);
            void act(uint64_t timePassedMillis, std::shared_ptr<EngineStorage> engineStorage) override;
            void spawnEnemy(std::shared_ptr<EngineStorage> engineStorage);
            void summonNextWave(std::shared_ptr<EngineStorage> engineStorage);
        	std::shared_ptr<WaveSample> findSample(std::string sampleName, std::shared_ptr<EngineStorage> engineStorage);

            Wave storage;
        };
    } // Inner
} // TDEngine