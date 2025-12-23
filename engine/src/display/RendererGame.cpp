#include "RendererGame.h"
#include "../inner/game_objects/GameStatus.h"
#include "../inner/game_objects/MapObject.h"

namespace TDEngine::Inner {

	RendererGame::RendererGame(sf::RenderWindow &window) : window(window) { std::cout << "Renderer initialized\n"; }

	const sf::Texture &RendererGame::getTexture(const std::string &path) { return textureCache.getTexture(path); }

	void RendererGame::renderFrame(const std::shared_ptr<GameStatus> &gameStat) {
		if (!gameStat)
			return;

		for (const auto &obj: gameStat->mapObjects) {

			const auto &texture = textureCache.getTexture(obj->texturePath);
			spriteCache.setTexture(texture, true);
			spriteCache.setPosition(static_cast<float>(obj->positionCoordinates.first) * TILE_SIZE,
									static_cast<float>(obj->positionCoordinates.second) * TILE_SIZE);

			window.draw(spriteCache);
		}
	}
} // namespace TDEngine::Inner
