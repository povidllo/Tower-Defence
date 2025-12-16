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
			mapView = window.getDefaultView();
			std::cout << "It is init\n";
		}

		void RendererGame::initMapDimensions(float worldWidth, float worldHeight) {
			mapView.setSize({worldWidth, worldHeight});
			mapView.setCenter({worldWidth / 2.0f, worldHeight / 2.0f});

			updateViewport();
		}

		void RendererGame::onResize() {
			updateViewport();
		}

		void RendererGame::updateViewport() {
			float windowRatio = static_cast<float>(window.getSize().x) / static_cast<float>(window.getSize().y);
			float viewRatio = mapView.getSize().x / mapView.getSize().y;

			float sizeX = 1.0f;
			float sizeY = 1.0f;
			float posX = 0.0f;
			float posY = 0.0f;

			if (windowRatio > viewRatio) {
				sizeX = viewRatio / windowRatio;
				posX = (1.0f - sizeX) / 2.0f;
			}
			else {
				sizeY = windowRatio / viewRatio;
				posY = (1.0f - sizeY) / 2.0f;
			}

			mapView.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
		}

		void RendererGame::renderFrame(const std::shared_ptr<GameStatus> &gameStat) {
			window.setView(mapView);

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

					sprite.setPosition({(float)obj->positionCoordinates.first * 32.0f, (float)obj->positionCoordinates.second * 32.0f});
					window.draw(sprite);
				}
			}

			// 3. (Опционально) Если нужно рисовать UI поверх черных полос или поверх карты:
			// window.setView(window.getDefaultView());
			// drawUI();
		}
	}
}