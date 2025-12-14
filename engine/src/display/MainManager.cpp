//
// Created by Mikle on 13.12.2025.
//

#include "MainManager.h"

namespace TDEngine {
	namespace Inner {
		MainManager::MainManager(Project &proj, unsigned int width, unsigned int height) :
			project(proj),
			width(width),
			height(height),
			engine(std::make_shared<Project>(project)),
			window(sf::RenderWindow(sf::VideoMode({width, height}), "Tower Defence")),
			renderer(window)
		{}

		void MainManager::mainLoop(std::string mapName) {
			std::shared_ptr<GameStatus> gameStatus;
			gameStatus = engine.startGame(mapName);
			while (true) {
				renderer.renderFrame(gameStatus);
				gameStatus = engine.gameStep();
			}
		}

	}
}
