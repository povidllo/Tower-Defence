//
// Created by Mikle on 13.12.2025.
//

#ifndef MAINMANAGER_H
#define MAINMANAGER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "../inner/core/Engine.h"
#include "Project.h"
#include "RendererGame.h"

namespace TDEngine {
	namespace Inner {
		class MainManager {
		public:
			sf::RenderWindow window;
			Project &project;
			unsigned int width;
			unsigned int height;
			Engine engine;
			RendererGame renderer;

			MainManager(Project &proj, unsigned int width, unsigned int height);
			void mainLoop(std::string mapName);
		};
	}
}



#endif //MAINMANAGER_H
