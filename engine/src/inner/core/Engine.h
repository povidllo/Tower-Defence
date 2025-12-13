#pragma once
#include <string>

#include "BoundaryDataTransfer.h"
#include "EngineStorage.h"
#include "FrameData.h"
#include "TickGenerator.h"

namespace TDEngine {
    namespace Inner {
        class Engine {
        public:
            Engine(std::shared_ptr<Project> pSample);
            void gameLoop();
            void startGame(const std::string& mapName);
        	void initMap();
            void endGame();
            bool solveNextAction();
        	void gameStep();
        private:
            std::shared_ptr<EngineStorage> storage;
            // std::optional<FrameData> curFrame;
            std::shared_ptr<BoundaryDataTransfer> boundaryDT;
            TickGenerator tickGen;
        };

    } // Inner
} // TDEngine