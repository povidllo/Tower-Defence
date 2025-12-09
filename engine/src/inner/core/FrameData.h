// #pragma once
// #include <vector>
//
// #include "../game_entities/GameStatus.h"
// #include "../game_entities/MapObject.h"
//
// namespace TDEngine {
//     namespace Inner {
//         class FrameData {
//         public:
//             FrameData(GameStatus status, std::vector<std::shared_ptr<MapObject>> mapObjects)
//                 : gameStatusCopy(status)
//             {
//                 for (const auto& mapObjectPtr : mapObjects) {
//                     mapObjectsCopy.push_back(*mapObjectPtr);
//                 }
//             }
//             GameStatus gameStatusCopy;
//             std::vector<MapObject> mapObjectsCopy;
//         };
//     } // Inner
// } // TDEngine