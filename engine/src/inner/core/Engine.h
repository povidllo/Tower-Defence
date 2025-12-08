#pragma once
#include <string>

#include "BoundaryDataTransfer.h"
#include "EngineStorage.h"
#include "FrameData.h"
#include "TickGenerator.h"
#include "../Zaglushki/ProjectSample.h"

namespace TDEngine {
    namespace Inner {
        class Engine {
        public:
            Engine(std::shared_ptr<ProjectSample> pSample);
            void gameLoop();
            void startGame(const std::string& mapName);
            void endGame(bool hasWon);
            bool solveNextAction();
        private:
            std::shared_ptr<EngineStorage> storage;
            std::shared_ptr<ProjectSample> curProject;
            std::optional<FrameData> curFrame;
            std::shared_ptr<BoundaryDataTransfer> boundaryDT;
            TickGenerator tickGen;
        };

    } // Inner
} // TDEngine