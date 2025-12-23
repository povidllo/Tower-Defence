#pragma once
#include <cstdint>

namespace TDEngine {
    namespace Inner {
        class GameStatus {
        public:
        	enum Status {
        		IDLE,
				PLAYING,
				LOST,
				WON
			};
            GameStatus(uint32_t currentHp, uint32_t currentGold)
                : currentHp(currentHp), currentGold(currentGold), status(IDLE) {}
            uint32_t currentHp;
            uint32_t currentGold;
            std::vector<std::shared_ptr<MapObject>> mapObjects;
        	Status status;
        };
    } // Inner
} // TDEngine