#pragma once
#include <string>

#include "BoundaryDataTransfer.h"
#include "EngineStorage.h"
#include "FrameData.h"

namespace TDEngine {
    namespace Inner {
        class Engine {
        public:
            void gameLoop();
            void startGame(std::string mapName);
            void endGame(bool hasWon);
            bool solveNextAction();
        private:
            EngineStorage storage;
            // Project curProject;
            FrameData curFrame;
            BoundaryDataTransfer boundaryDT;
        };

    } // Inner
} // TDEngine