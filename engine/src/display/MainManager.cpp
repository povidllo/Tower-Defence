//
// Created by Mikle on 13.12.2025.
//

#include "MainManager.h"
#include <SFML/Graphics/Texture.hpp>

namespace TDEngine {
	namespace Inner {

		std::string MainManager::getMapBackgroundImgPath(const std::string mapName) {
			for (const auto &map: project.getMaps()) {
				if (map->getName() == mapName) {
					return map->getFinalMapImagePath();
				}
			}

			return "";
		}

		MainManager::MainManager(Project &proj, unsigned int width, unsigned int height) :
			project(proj), width(width), height(height), engine(std::make_shared<Project>(project)),
			window(sf::RenderWindow(sf::VideoMode(width, height), "Tower Defence")), renderer(window) {
			std::cout << "main manager init\n";
		}

		void MainManager::mainLoop(std::string mapName) {
			std::shared_ptr<GameStatus> gameStatus;
			gameStatus = engine.startGame(mapName);
			sf::Sprite mapBackground(renderer.textureCache.getTexture(getMapBackgroundImgPath(mapName)));
			mapBackground.setPosition(0, 0);

			while (window.isOpen()) {
				sf::Event event;
				while (window.pollEvent(event)) {

					if (event.type == sf::Event::Closed)
						window.close();
				}

				window.clear();
				window.draw(mapBackground);
				renderer.renderFrame(gameStatus);
				window.display();

				gameStatus = engine.gameStep();
			}
		}

	} // namespace Inner
} // namespace TDEngine
