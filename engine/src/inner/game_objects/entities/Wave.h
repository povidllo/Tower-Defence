#pragma once

#include "../../../../../editor/include/Entity/WaveSample.h"
namespace TDEngine {
    namespace Inner {
        class Wave : public WaveSample{
        public:
            explicit Wave(WaveSample sample)
                : WaveSample(sample) {}
            uint64_t timeAfterLastSpawn;
            uint32_t enemiesSpawned;
            uint32_t spawningIndex;
        };
    } // Inner
} // TDEngine