//
// Created by Mikle on 13.12.2025.
//

#include "MainManager.h"
#include <SFML/Graphics/Texture.hpp>
#include <vector>

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
			std::vector<std::pair<sf::Texture, std::string>> showUpgrade;

			while (window.isOpen()) {
				sf::Event event;
				while (window.pollEvent(event)) {

					if (event.type == sf::Event::Closed)
						window.close();

					if (event.type == sf::Event::MouseButtonPressed) {
						if (event.mouseButton.button == sf::Mouse::Left) {
							showUpgrade.clear();

							sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
							sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

							for (const auto &obj: gameStatus->mapObjects) {
								if (obj->type != MapObjectTypes::Tower)
									continue;

								if (worldPos.x > obj->positionCoordinates.first * 32 &&
									worldPos.x < obj->positionCoordinates.first * 32 + 32 &&
									worldPos.y > obj->positionCoordinates.second * 32 &&
									worldPos.y < obj->positionCoordinates.second * 32 + 32) {
									std::cout << "tower: " << obj << "\n";
									std::cout << "by x: " << obj->positionCoordinates.first * 32 + 16
											  << " | y: " << obj->positionCoordinates.second * 32 + 16 << std::endl;
									std::cout << "you can upgrade it to \n";
									auto towerPtr = std::static_pointer_cast<TowerActions>(obj);
									for (const auto &upg: towerPtr->storage.getUpgradeNames()) {

										std::cout << upg << std::endl;
										for (const auto &tower: project.getTowers()) {
											if (tower->getName() == upg) {
												showUpgrade.push_back(std::pair{
														renderer.textureCache.getTexture(tower->getTowerTexturePath()),
														upg});
												break;
											}
										}
									}

									break;
								}
							}
						}
					}
				}

				window.clear();

				window.draw(mapBackground);
				renderer.renderFrame(gameStatus);
				sf::Sprite upgTowSprite;
				for (int i = 0; i < showUpgrade.size(); i++) {
					upgTowSprite.setTexture(showUpgrade[i].first);
					upgTowSprite.setPosition(600, (float) i * 32);
					window.draw(upgTowSprite);
				}

				window.display();

				gameStatus = engine.gameStep();
			}
		}

	} // namespace Inner
} // namespace TDEngine
