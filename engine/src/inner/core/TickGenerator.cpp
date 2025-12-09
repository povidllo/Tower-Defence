#include "TickGenerator.h"


namespace TDEngine {
    namespace Inner {
        TickGenerator::TickGenerator(std::chrono::time_point<std::chrono::steady_clock> startTime)
            : lastTickTime(startTime) {}

        void TickGenerator::tick(const std::shared_ptr<EngineStorage> engineStorage) {
            auto curTickTime = std::chrono::steady_clock::now();
            uint64_t tickIntervalTImeMillis =
                std::chrono::duration_cast<std::chrono::milliseconds>(curTickTime - lastTickTime).count();

            for (auto & acting : engineStorage->getEverythingActing()) {
                acting->act(tickIntervalTImeMillis, engineStorage);
            }

            lastTickTime = curTickTime;
        }

        void TickGenerator::resetTime(std::chrono::time_point<std::chrono::steady_clock> cur) {
            lastTickTime = cur;
        }
    } // Inner
} // TDEngine