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
					std::cout << obj->texturePath << std::endl;

					if (obj->type != neededType) {
						continue;
					}

					// ВАЖНО: textureCache должен быть доступен
					sf::Sprite sprite(textureCache.getTexture(obj->texturePath));

					// ВРЕМЕННО: Создаем заглушку, если textureCache не подключен в этом примере
					// sf::RectangleShape sprite({32.f, 32.f});
					// sprite.setFillColor(sf::Color::Red);

					sprite.setPosition({(float)obj->positionCoordinates.first * 32.0f - 16.0f, (float)obj->positionCoordinates.second * 32.0f - 16.0f});
					window.draw(sprite);
				}
			}

			// 3. (Опционально) Если нужно рисовать UI поверх черных полос или поверх карты:
			// window.setView(window.getDefaultView());
			// drawUI();
		}
	}
}