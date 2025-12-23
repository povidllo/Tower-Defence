#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <queue>
#include <vector>

#include "../inner/core/Engine.h"
#include "../inner/game_objects/MapObject.h"
#include "../inner/player_actions/TowerUpgradeAction.h"
#include "Project.h"
#include "RendererGame.h"

namespace TDEngine::Inner {

	// Добавили состояние GAME_OVER
	enum class AppState { MENU, GAME, GAME_OVER };

	class MainManager {
	public:
		MainManager(Project &proj, unsigned int width, unsigned int height);

		void run();

	private:
		void processEvents();
		void update(sf::Time dt);
		void render();

		void handleMenuClick(int mouseX, int mouseY);
		void handleGameClick(int mouseX, int mouseY);
		// Обработчик клика на экране конца игры
		void handleGameOverClick();

		void initMenu();
		void startGameLevel(const std::string &mapName);
		std::string getMapBackgroundImgPath(const std::string &mapName);

		sf::RenderWindow window;
		Project &project;
		Engine engine;
		RendererGame renderer;
		AppState state;

		// Данные игры
		std::shared_ptr<GameStatus> gameStatus;
		sf::Sprite backgroundSprite;
		std::shared_ptr<TowerUpgradeAction> playerAction;
		std::shared_ptr<MapObject> selectedTower;
		std::vector<UpgradeOption> currentUpgradeOptions;

		// Результат игры
		bool wasVictory = false;

		// Данные меню
		std::vector<MenuButton> menuButtons;
	};
} // namespace TDEngine::Inner
