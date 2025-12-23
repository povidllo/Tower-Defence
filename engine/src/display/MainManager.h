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


#include "../inner/game_objects/MapObject.h"

namespace TDEngine::Inner {


	struct UpgradeOption {
		const sf::Texture *texture;
		std::string name;
		sf::FloatRect bounds;
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

		std::shared_ptr<TowerUpgradeAction> playerAction;
		std::shared_ptr<MapObject> selectedTower;


		std::vector<UpgradeOption> currentUpgradeOptions;
	};
} // namespace TDEngine::Inner
