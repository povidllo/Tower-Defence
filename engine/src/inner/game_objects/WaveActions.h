#pragma once
#include "IActing.h"
#include "entities/Wave.h"

namespace TDEngine {
    namespace Inner {
        class WaveActions : public IActing{
        public:
            explicit WaveActions(WaveSample sample);
            void spawnEnemy();
            void summonNextWave();

            Wave storage;
        };
    } // Inner
} // TDEngine