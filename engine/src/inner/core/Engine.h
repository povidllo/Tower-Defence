#pragma once
#include <string>

#include "EngineStorage.h"
#include "FrameData.h"
#include "TickGenerator.h"
#include "../player_actions/IPlayerAction.h"

namespace TDEngine {
    namespace Inner {
		class Engine {
        public:
            Engine(std::shared_ptr<Project> pSample);
            std::shared_ptr<GameStatus> gameStep(std::shared_ptr<IPlayerAction> action);
            std::shared_ptr<GameStatus> startGame(const std::string& mapName, bool cooperativePlay = false);
        	void initMap();
            void checkForVictory();
        	std::vector<std::shared_ptr<EnginePlayer>> getAllPlayers();

        private:
            std::shared_ptr<EngineStorage> storage;
            TickGenerator tickGen;
        };

    } // Inner
} // TDEngine