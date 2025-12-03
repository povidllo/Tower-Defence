#include "Engine.h"

namespace TDEngine {
    namespace Inner {
        Engine::Engine(std::shared_ptr<ProjectSample> pSample)
            : storage(nullptr), curProject(pSample), curFrame(std::nullopt),
            boundaryDT(std::shared_ptr<BoundaryDataTransfer>(new BoundaryDataTransfer(this, curProject))),
            tickGen(std::chrono::steady_clock::now())
        {
        }

        void Engine::gameLoop() {
            while (true) {
                while (solveNextAction());
                tickGen.tick(storage->getEverythingActing());
                auto frame = new FrameData(storage->getGameStatus(), storage->getAllMapObjects());
                boundaryDT->setNextFrame(*frame);
            }
        }

        void Engine::startGame(std::string mapName) {
            MapSample map; //Расписать получение карты по имени из проекта
            storage = std::make_shared<EngineStorage>(map);
            gameLoop();
        }

        void Engine::endGame(bool hasWon) {

        }

        bool Engine::solveNextAction() {
            std::optional<std::shared_ptr<IPlayerAction>> action = boundaryDT->extractPlayerAction();
            if (action.has_value()) {
                action.value()->MakeAction();
                return true;
            }
            return false;
        }





    } // Inner
} // TDEngine