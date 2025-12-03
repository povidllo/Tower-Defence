#pragma once
#include <vector>

#include "../game_entities/GameStatus.h"
#include "../game_entities/MapObject.h"

namespace TDEngine {
    namespace Inner {
        class FrameData {
        public:
            //Тут дописать
            FrameData(GameStatus status, std::vector<std::shared_ptr<MapObject>>) {

            }
            GameStatus gameStatusCopy;
            std::vector<MapObject> mapObjectsCopy;
        };
    } // Inner
} // TDEngine