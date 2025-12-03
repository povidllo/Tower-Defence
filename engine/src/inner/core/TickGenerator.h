#pragma once
#include <vector>
#include <chrono>
#include "../game_entities/IActing.h"

namespace TDEngine {
    namespace Inner {
        class TickGenerator {
        public:
            void tick(std::vector<IActing>& everythingActing);

        private:
            std::chrono::time_point<std::chrono::steady_clock> lastTickTime;
        };
    } // Inner
} // TDEngine