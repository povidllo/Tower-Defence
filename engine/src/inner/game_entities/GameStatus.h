#pragma once
#include <cstdint>

namespace TDEngine {
    namespace Inner {
        class GameStatus {
        public:
            GameStatus(uint32_t currentHp, uint32_t currentGold)
                : currentHp(currentHp), currentGold(currentGold) {}
            uint32_t currentHp;
            uint32_t currentGold;
        };
    } // Inner
} // TDEngine