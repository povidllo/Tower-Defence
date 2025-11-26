#include "TickGenerator.h"

namespace TDEngine {
    namespace Inner {
        void TickGenerator::tick(std::vector<IActing>& everythingActing) {
            auto curTickTime = std::chrono::steady_clock::now();
            uint64_t tickIntervalTIme =
                std::chrono::duration_cast<std::chrono::milliseconds>(curTickTime - lastTickTime).count();

            for (auto & acting : everythingActing) {
                acting.act(tickIntervalTIme);
            }

            lastTickTime = curTickTime;
        }
    } // Inner
} // TDEngine