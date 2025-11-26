#pragma once
#include <cstdint>

namespace TDEngine {
    namespace Inner {
        class GameStatus {
        public:
            uint32_t currentHp;
            uint32_t currentGold;
        };
    } // Inner
} // TDEngine