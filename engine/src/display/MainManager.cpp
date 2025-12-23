#include "MainManager.h"
#include <iostream>
#include <memory>

namespace TDEngine::Inner {

	MainManager::MainManager(Project &proj, unsigned int width, unsigned int height) :
		project(proj), engine(std::make_shared<Project>(project)),
		window(sf::VideoMode(width, height), "Tower Defence", sf::Style::Titlebar | sf::Style::Close),
		renderer(window) {
		window.setFramerateLimit(60);
	}

	std::string MainManager::getMapBackgroundImgPath(const std::string &mapName) {
		for (const auto &map: project.getMaps()) {
			if (map->getName() == mapName) {
				return map->getFinalMapImagePath();
			}
		}
		return "";
	}

	void MainManager::run(const std::string &mapName) {
		gameStatus = engine.startGame(mapName);

		std::string bgPath = getMapBackgroundImgPath(mapName);
		backgroundSprite.setTexture(renderer.getTexture(bgPath));

		sf::Clock clock;

		while (window.isOpen()) {
			if (gameStatus->status == gameStatus->LOST || gameStatus->status == gameStatus->WON) {
				std::cout << "cong\n";
				break;
			}
			sf::Time dt = clock.restart();

			processEvents();
			update(dt);
			render();
		}
	}

	void MainManager::processEvents() {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			} else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					handleMouseClick(event.mouseButton.x, event.mouseButton.y);
				}
			}
		}
	}

	void MainManager::handleMouseClick(int mouseX, int mouseY) {

		sf::Vector2f worldPos = window.mapPixelToCoords({mouseX, mouseY});


		if (selectedTower) {
			for (const auto &option: currentUpgradeOptions) {
				if (option.bounds.contains(worldPos)) {


					std::cout << "[ACTION] Tower at {" << selectedTower->positionCoordinates.first << ";"
							  << selectedTower->positionCoordinates.second << "}"
							  << " wants to upgrade to: " << option.name << std::endl;
					TowerUpgradeAction newUpgrade(option.name, std::static_pointer_cast<TowerActions>(selectedTower));
					playerAction = std::make_shared<TowerUpgradeAction>(
							option.name, std::static_pointer_cast<TowerActions>(selectedTower));


					return;
				}
			}
		}

		bool clickedOnTower = false;

		for (const auto &obj: gameStatus->mapObjects) {
			if (obj->type != MapObjectTypes::Tower)
				continue;

			float objX = static_cast<float>(obj->positionCoordinates.first) * RendererGame::TILE_SIZE;
			float objY = static_cast<float>(obj->positionCoordinates.second) * RendererGame::TILE_SIZE;

			sf::FloatRect objBounds(objX, objY, RendererGame::TILE_SIZE, RendererGame::TILE_SIZE);

			if (objBounds.contains(worldPos)) {
				selectedTower = obj;
				clickedOnTower = true;

				std::cout << "Selected tower at " << obj->positionCoordinates.first << ":"
						  << obj->positionCoordinates.second << std::endl;

				currentUpgradeOptions.clear();

				auto towerPtr = std::static_pointer_cast<TowerActions>(obj);

				int index = 0;
				float uiStartX = 600.0f;

				for (const auto &upgName: towerPtr->storage.getUpgradeNames()) {
					for (const auto &towerConfig: project.getTowers()) {
						if (towerConfig->getName() == upgName) {

							float btnY = static_cast<float>(index) * 40.0f;
							sf::FloatRect btnRect(uiStartX, btnY, 32.0f, 32.0f);

							currentUpgradeOptions.push_back(
									{&renderer.getTexture(towerConfig->getTowerTexturePath()), upgName, btnRect});

							index++;
							break;
						}
					}
				}
				return;
			}
		}

		if (!clickedOnTower) {
			selectedTower = nullptr;
			currentUpgradeOptions.clear();
			std::cout << "Selection cleared." << std::endl;
		}
	}

	void MainManager::update(sf::Time dt) {
		static sf::Time timeAccumulator = sf::Time::Zero;
		timeAccumulator += dt;
		const sf::Time TIME_PER_FRAME = sf::seconds(1.f / 60.f);

		while (timeAccumulator > TIME_PER_FRAME) {
			gameStatus = engine.gameStep(playerAction);
			playerAction = nullptr;
			timeAccumulator -= TIME_PER_FRAME;
		}
	}

	void MainManager::render() {
		window.clear();

		window.draw(backgroundSprite);
		renderer.renderFrame(gameStatus);

		sf::Sprite uiSprite;
		for (const auto &opt: currentUpgradeOptions) {
			if (opt.texture) {
				uiSprite.setTexture(*opt.texture);
				uiSprite.setPosition(opt.bounds.left, opt.bounds.top);
				window.draw(uiSprite);
			}
		}

		window.display();
	}
} // namespace TDEngine::Inner
