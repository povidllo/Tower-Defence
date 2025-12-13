//
// Created by Mikle on 13.12.2025.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

namespace TDEngine {
	namespace Inner {
		class Renderer {
		public:
			unsigned int windowWidth{800};
			unsigned int windowHeight{600};
			sf::RenderWindow window;
			Renderer(int width, int height);
			void renderOneObj();
			void displayAll();
		};
	}
}


#endif //RENDERER_H
