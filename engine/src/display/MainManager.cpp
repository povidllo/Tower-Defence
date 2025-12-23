#include "MainManager.h"
#include <iostream>
#include <memory>

namespace TDEngine::Inner {

	MainManager::MainManager(Project &proj, unsigned int width, unsigned int height) :
		project(proj), engine(std::make_shared<Project>(project)),
		window(sf::VideoMode(width, height), "Tower Defence", sf::Style::Titlebar | sf::Style::Close), renderer(window),
		state(AppState::MENU) {
		window.setFramerateLimit(60);
		initMenu();
	}

	void MainManager::initMenu() {
		menuButtons.clear();
		auto maps = project.getMaps();

		float btnWidth = 300.0f;
		float btnHeight = 60.0f;
		float startY = 200.0f;
		float spacing = 25.0f;
		float centerX = (window.getSize().x - btnWidth) / 2.0f;

		int index = 0;
		for (const auto &map: maps) {
			std::string name = map->getName();
			sf::FloatRect rect(centerX, startY + index * (btnHeight + spacing), btnWidth, btnHeight);
			menuButtons.push_back({name, rect, false});
			index++;
		}
	}

	void MainManager::startGameLevel(const std::string &mapName) {
		std::cout << "[INFO] Loading map: " << mapName << std::endl;
		gameStatus = engine.startGame(mapName);
		std::string bgPath = getMapBackgroundImgPath(mapName);
		backgroundSprite.setTexture(renderer.getTexture(bgPath));
		selectedTower = nullptr;
		currentUpgradeOptions.clear();
		playerAction = nullptr;
		wasVictory = false;
		state = AppState::GAME;
	}

	std::string MainManager::getMapBackgroundImgPath(const std::string &mapName) {
		for (const auto &map: project.getMaps()) {
			if (map->getName() == mapName) {
				return map->getFinalMapImagePath();
			}
		}
		return "";
	}

	void MainManager::run() {
		sf::Clock clock;
		while (window.isOpen()) {
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
					if (state == AppState::MENU) {
						handleMenuClick(event.mouseButton.x, event.mouseButton.y);
					} else if (state == AppState::GAME) {
						handleGameClick(event.mouseButton.x, event.mouseButton.y);
					} else if (state == AppState::GAME_OVER) {
						handleGameOverClick();
					}
				}
			} else if (event.type == sf::Event::MouseMoved && state == AppState::MENU) {
				sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
				for (auto &btn: menuButtons) {
					btn.isHovered = btn.bounds.contains(mousePos);
				}
			}
		}
	}

	void MainManager::handleMenuClick(int mouseX, int mouseY) {
		sf::Vector2f worldPos = window.mapPixelToCoords({mouseX, mouseY});
		for (const auto &btn: menuButtons) {
			if (btn.bounds.contains(worldPos)) {
				startGameLevel(btn.text);
				return;
			}
		}
	}

	void MainManager::handleGameOverClick() {
		state = AppState::MENU;
		gameStatus = nullptr;
	}

	void MainManager::handleGameClick(int mouseX, int mouseY) {
		sf::Vector2f worldPos = window.mapPixelToCoords({mouseX, mouseY});

		// 1. Проверка кликов по UI (Правая панель - Sidebar)
		if (worldPos.x >= RendererGame::UI_SIDEBAR_X) {
			if (selectedTower) {
				for (const auto &option: currentUpgradeOptions) {
					if (option.bounds.contains(worldPos)) {
						playerAction = std::make_shared<TowerUpgradeAction>(
								option.name, std::static_pointer_cast<TowerActions>(selectedTower));
						selectedTower = nullptr;
						currentUpgradeOptions.clear();
						return;
					}
				}
			}
			return;
		}

		// Если клик по верхней панели
		if (worldPos.y <= RendererGame::UI_TOP_BAR_HEIGHT) {
			return;
		}

		// 2. Проверка кликов по карте
		sf::Vector2u bgSize =
				backgroundSprite.getTexture() ? backgroundSprite.getTexture()->getSize() : sf::Vector2u(0, 0);
		sf::Vector2f mapOffset = RendererGame::getMapOffset(window.getSize(), bgSize);

		float localX = worldPos.x - mapOffset.x;
		float localY = worldPos.y - mapOffset.y;

		bool clickedOnTower = false;

		if (localX >= 0 && localY >= 0 && localX <= bgSize.x && localY <= bgSize.y) {
			for (const auto &obj: gameStatus->mapObjects) {
				if (obj->type != MapObjectTypes::Tower)
					continue;

				float objX = static_cast<float>(obj->positionCoordinates.first) * RendererGame::TILE_SIZE;
				float objY = static_cast<float>(obj->positionCoordinates.second) * RendererGame::TILE_SIZE;

				sf::FloatRect objBounds(objX, objY, RendererGame::TILE_SIZE, RendererGame::TILE_SIZE);

				if (objBounds.contains(localX, localY)) {
					selectedTower = obj;
					clickedOnTower = true;
					currentUpgradeOptions.clear();

					auto towerPtr = std::static_pointer_cast<TowerActions>(obj);
					int index = 0;

					// --- ИСПРАВЛЕНИЕ ЛОГИКИ РАЗМЕЩЕНИЯ КНОПОК ---
					// Вычисляем ширину доступной области внутри сайдбара
					float sidebarInnerWidth =
							window.getSize().x - RendererGame::UI_SIDEBAR_X - (2 * RendererGame::UI_PADDING);

					// Делаем кнопку почти во всю ширину сайдбара (с небольшими отступами)
					float btnMarginX = 10.0f;
					float btnWidth = sidebarInnerWidth - (2 * btnMarginX);
					float btnHeight = 50.0f; // Фиксированная высота кнопки-карточки

					float startX = RendererGame::UI_SIDEBAR_X + RendererGame::UI_PADDING + btnMarginX;
					float startY = 80.0f; // Отступ сверху (под заголовком)

					for (const auto &upgName: towerPtr->storage.getUpgradeNames()) {
						for (const auto &towerConfig: project.getTowers()) {
							if (towerConfig->getName() == upgName) {
								float btnY = startY + (index * (btnHeight + 10.0f)); // 10.0f - отступ между кнопками

								currentUpgradeOptions.push_back(
										{&renderer.getTexture(towerConfig->getTowerTexturePath()), upgName,
										 sf::FloatRect(startX, btnY, btnWidth, btnHeight)});
								index++;
								break;
							}
						}
					}
					return;
				}
			}
		}

		if (!clickedOnTower) {
			selectedTower = nullptr;
			currentUpgradeOptions.clear();
		}
	}

	void MainManager::update(sf::Time dt) {
		if (state != AppState::GAME)
			return;

		static sf::Time timeAccumulator = sf::Time::Zero;
		timeAccumulator += dt;
		const sf::Time TIME_PER_FRAME = sf::seconds(1.f / 60.f);

		if (gameStatus && (gameStatus->status == gameStatus->LOST || gameStatus->status == gameStatus->WON)) {
			wasVictory = (gameStatus->status == gameStatus->WON);
			state = AppState::GAME_OVER;
			return;
		}

		while (timeAccumulator > TIME_PER_FRAME) {
			if (gameStatus) {
				gameStatus = engine.gameStep(playerAction);
			}
			playerAction = nullptr;
			timeAccumulator -= TIME_PER_FRAME;
		}
	}

	void MainManager::render() {
		if (state == AppState::MENU) {
			renderer.renderMenu(menuButtons);
			window.display();
		} else if (state == AppState::GAME) {
			window.clear(sf::Color(20, 20, 25));
			renderer.renderScene(gameStatus, backgroundSprite);
			renderer.renderUI(gameStatus, currentUpgradeOptions);
			window.display();
		} else if (state == AppState::GAME_OVER) {
			window.clear(sf::Color(20, 20, 25));
			renderer.renderScene(gameStatus, backgroundSprite);
			renderer.renderUI(gameStatus, currentUpgradeOptions);
			renderer.renderGameOver(wasVictory);
			window.display();
		}
	}
} // namespace TDEngine::Inner
