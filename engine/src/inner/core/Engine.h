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
            std::shared_ptr<GameStatus> gameStep();
            std::shared_ptr<GameStatus> startGame(const std::string& mapName);
        	void initMap();
            void endGame();
            bool solveNextAction();
        	bool isPlaying();
        private:
            std::shared_ptr<EngineStorage> storage;
            std::shared_ptr<BoundaryDataTransfer> boundaryDT;
            TickGenerator tickGen;
        };

    } // Inner
} // TDEngine