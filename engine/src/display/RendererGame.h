//
// Created by Mikle on 13.12.2025.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "TextureCache.h"
#include "../inner/game_objects/MapObject.h"
#include "../inner/game_objects/GameStatus.h"

namespace TDEngine {
	namespace Inner {
		class RendererGame {
		public:
			TextureCache textureCache;
			void renderFrame(const std::shared_ptr<GameStatus> &gameStat);
			RendererGame(sf::RenderWindow &window);
			void initMapDimensions(float worldWidth, float worldHeight);
			void onResize();

		private:
			sf::RenderWindow &window;
			sf::View mapView;
			void updateViewport();
		};
	}
}


#endif //RENDERER_H
