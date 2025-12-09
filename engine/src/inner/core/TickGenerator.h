#pragma once
#include <vector>
#include <chrono>
#include "../game_objects/IActing.h"

namespace TDEngine {
    namespace Inner {
        class TickGenerator {
        public:
            explicit TickGenerator(std::chrono::time_point<std::chrono::steady_clock> startTime);
            void tick(const std::vector<std::shared_ptr<IActing>> everythingActing);
            void resetTime(std::chrono::time_point<std::chrono::steady_clock> startTime);

        private:
            std::chrono::time_point<std::chrono::steady_clock> lastTickTime;
        };
    } // Inner
} // TDEngine