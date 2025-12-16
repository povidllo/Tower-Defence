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
			renderer(window) {
			std::cout << "main manager init\n";
		}

		void MainManager::mainLoop(std::string mapName) {
			std::shared_ptr<GameStatus> gameStatus;
			gameStatus = engine.startGame(mapName);

			float mapPixelWidth = 15 * 32.0f;
			float mapPixelHeight = 10 * 32.0f;

			renderer.initMapDimensions(mapPixelWidth, mapPixelHeight);


			while (window.isOpen()) {
				while (const auto event = window.pollEvent()) {
					if (event->is<sf::Event::Closed>()) {
						window.close();
					}
					// [ВАЖНО] Обработка ресайза для SFML 3
					else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
						// Сообщаем рендереру, что окно изменилось
						renderer.onResize();
					}
				}

				window.clear(); // Очистка черным цветом (будет на полосах)
				renderer.renderFrame(gameStatus);
				window.display();

				gameStatus = engine.gameStep();
			}
		}

	}
}
