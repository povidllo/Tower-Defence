#include "RendererGame.h"
#include "../inner/game_objects/GameStatus.h"
#include "../inner/game_objects/MapObject.h"

namespace TDEngine::Inner {

	RendererGame::RendererGame(sf::RenderWindow &window) : window(window) { std::cout << "Renderer initialized\n"; }

	const sf::Texture &RendererGame::getTexture(const std::string &path) { return textureCache.getTexture(path); }

	void RendererGame::renderFrame(const std::shared_ptr<GameStatus> &gameStat) {
		if (!gameStat)
			return;

		// Оптимизация: вместо создания спрайта в цикле, используем один и меняем его свойства
		for (const auto &obj: gameStat->mapObjects) {
			// Если у вас есть порядок отрисовки (земля -> башни -> враги -> пули),
			// его лучше обеспечить сортировкой вектора mapObjects в Engine,
			// либо здесь, но один раз.

			const auto &texture = textureCache.getTexture(obj->texturePath);
			spriteCache.setTexture(texture, true); // true сбрасывает rect текстуры

			// Позиционирование
			spriteCache.setPosition(static_cast<float>(obj->positionCoordinates.first) * TILE_SIZE,
									static_cast<float>(obj->positionCoordinates.second) * TILE_SIZE);

			window.draw(spriteCache);
		}
	}
} // namespace TDEngine::Inner
