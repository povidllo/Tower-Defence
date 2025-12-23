#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include "TextureCache.h"
// Forward declaration, чтобы не тянуть тяжелые хедеры сюда
namespace TDEngine::Inner {
	struct GameStatus;
}

namespace TDEngine::Inner {
	class RendererGame {
	public:
		static constexpr float TILE_SIZE = 32.0f; // Глобальная константа для визуализации

		explicit RendererGame(sf::RenderWindow &window);

		void renderFrame(const std::shared_ptr<GameStatus> &gameStat);

		// Метод для получения текстуры (прокси к кэшу)
		const sf::Texture &getTexture(const std::string &path);

	private:
		sf::RenderWindow &window;
		TextureCache textureCache;
		sf::Sprite spriteCache; // Один спрайт для переиспользования (оптимизация)
	};
} // namespace TDEngine::Inner
