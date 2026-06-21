#pragma once

#include "../../../../../editor/include/Entity/WaveSample.h"
#include "WaveChain.h"
namespace TDEngine {
    namespace Inner {
        class Wave : public WaveSample{
        public:
            explicit Wave(WaveSample sample)
                : WaveSample(sample) {}
            uint64_t timeAfterLastSpawn;
            uint64_t delayTime;
            uint32_t enemiesSpawned;
            uint32_t spawningIndex;
        	std::shared_ptr<WaveChain> chain;
        	int chainIndex;
        };
    } // Inner
} // TDEngine