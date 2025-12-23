#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <queue>
#include <vector>

#include "../inner/core/Engine.h"
#include "../inner/player_actions/TowerUpgradeAction.h"
#include "Project.h"
#include "RendererGame.h"

// Предполагаем, что TowerActions наследуется от MapObject
// Если нет, замените MapObject на нужный базовый класс
#include "../inner/game_objects/MapObject.h"

namespace TDEngine::Inner {

	// Структура для кнопки улучшения
	struct UpgradeOption {
		const sf::Texture *texture; // Указатель на текстуру (не владеем ей)
		std::string name; // Имя улучшения (например, "MegaTower")
		sf::FloatRect bounds; // Прямоугольник для проверки клика (x, y, width, height)
	};

	class MainManager {
	public:
		MainManager(Project &proj, unsigned int width, unsigned int height);
		void run(const std::string &mapName);

	private:
		void processEvents();
		void update(sf::Time dt);
		void render();
		void handleMouseClick(int mouseX, int mouseY);
		std::string getMapBackgroundImgPath(const std::string &mapName);

		sf::RenderWindow window;
		Project &project;
		Engine engine;
		RendererGame renderer;

		std::shared_ptr<GameStatus> gameStatus;
		sf::Sprite backgroundSprite;

		// --- НОВЫЕ ПОЛЯ ---
		// Храним указатель на выбранную башню.
		// Используем weak_ptr или shared_ptr в зависимости от архитектуры,
		// но shared_ptr безопаснее, если объект может удалиться.
		std::shared_ptr<TowerUpgradeAction> playerAction;
		std::shared_ptr<MapObject> selectedTower;

		// Список текущих кнопок апгрейда
		std::vector<UpgradeOption> currentUpgradeOptions;
	};
} // namespace TDEngine::Inner
