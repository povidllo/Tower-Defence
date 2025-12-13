//
// Created by Mikle on 13.12.2025.
//

#include "renderer.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "../inner/core/Engine.h"

namespace TDEngine {
	namespace Inner {
		// Renderer::Renderer(unsigned int width, unsigned int height) : windowWidth(width), windowHeight(height), window(sf::VideoMode(800, 600),
		// 				  "SFML Image Resizing") {
		// }

		Renderer::Renderer(int width, int height) {
		std::cout << "It is init\n";
		}

		void renderOneObj(const std::shared_ptr<MapObject> &draw_obj) {
			 std::cout << "I drew obj on " << draw_obj->positionCoordinates.first << "|" << draw_obj->positionCoordinates.second;
		}

	}
}