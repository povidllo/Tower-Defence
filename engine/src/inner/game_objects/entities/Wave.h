#pragma once

#include "../../../../../editor/include/Entity/WaveSample.h"
namespace TDEngine {
    namespace Inner {
        class Wave : public WaveSample{
        public:
            uint64_t timeAfterLastSpawn;
            uint32_t enemiesSpawned;
        };
    } // Inner
} // TDEngine