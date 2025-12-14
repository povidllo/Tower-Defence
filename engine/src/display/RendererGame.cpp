//
// Created by Mikle on 13.12.2025.
//

#include "RendererGame.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "../inner/core/Engine.h"

namespace TDEngine {
	namespace Inner {
		RendererGame::RendererGame(sf::RenderWindow &window) : window(window) {
			std::cout << "It is init\n";
		}

		void RendererGame::renderFrame(const std::shared_ptr<GameStatus> &gameStat) {

			for (const auto neededType : std::vector<MapObjectTypes> {MapObjectTypes::Tower, MapObjectTypes::Enemy, MapObjectTypes::Projectile}) {
				for (const auto& obj : gameStat->mapObjects) {
					if (obj->type != neededType) {
						continue;
					}

					sf::Sprite sprite(textureCache.getTexture(obj->texturePath));
					sprite.setPosition({(float)obj->positionCoordinates.first, (float)obj->positionCoordinates.second});
					window.draw(sprite);
				}
			}
		}

	}
}