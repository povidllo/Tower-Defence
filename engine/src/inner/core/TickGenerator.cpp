#include "TickGenerator.h"

namespace TDEngine {
    namespace Inner {
        TickGenerator::TickGenerator(std::chrono::time_point<std::chrono::steady_clock> startTime)
            : lastTickTime(startTime) {}

        void TickGenerator::tick(const std::vector<std::shared_ptr<IActing>> everythingActing) {
            auto curTickTime = std::chrono::steady_clock::now();
            uint64_t tickIntervalTImeMillis =
                std::chrono::duration_cast<std::chrono::milliseconds>(curTickTime - lastTickTime).count();

            for (auto & acting : everythingActing) {
                acting->act(tickIntervalTImeMillis);
            }

            lastTickTime = curTickTime;
        }

        void TickGenerator::resetTime(std::chrono::time_point<std::chrono::steady_clock> cur) {
            lastTickTime = cur;
        }
    } // Inner
} // TDEngine