#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include "TextureCache.h"

namespace TDEngine::Inner {
	struct GameStatus;
}

namespace TDEngine::Inner {
	class RendererGame {
	public:
		static constexpr float TILE_SIZE = 32.0f;

		explicit RendererGame(sf::RenderWindow &window);

		void renderFrame(const std::shared_ptr<GameStatus> &gameStat);

		const sf::Texture &getTexture(const std::string &path);

	private:
		sf::RenderWindow &window;
		TextureCache textureCache;
		sf::Sprite spriteCache;
	};
} // namespace TDEngine::Inner
