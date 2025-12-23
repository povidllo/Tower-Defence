#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include "TextureCache.h"

namespace TDEngine::Inner {
	struct GameStatus;

	struct UpgradeOption {
		const sf::Texture *texture;
		std::string name;
		sf::FloatRect bounds;
	};

	struct MenuButton {
		std::string text;
		sf::FloatRect bounds;
		bool isHovered = false;
	};

	class RendererGame {
	public:
		static constexpr float TILE_SIZE = 32.0f;

		// Логические границы (для расчетов кликов)
		static constexpr float UI_SIDEBAR_X = 600.0f;
		static constexpr float UI_TOP_BAR_HEIGHT = 60.0f; // Чуть увеличим для красоты

		// Визуальные настройки
		static constexpr float UI_PADDING = 10.0f; // Отступ между панелями
		static constexpr float CORNER_RADIUS = 15.0f; // Радиус скругления

		explicit RendererGame(sf::RenderWindow &window);

		void renderScene(const std::shared_ptr<GameStatus> &gameStat, const sf::Sprite &background);
		void renderUI(const std::shared_ptr<GameStatus> &gameStat, const std::vector<UpgradeOption> &upgradeOptions);
		void renderGameOver(bool victory);
		void renderMenu(const std::vector<MenuButton> &buttons);

		[[nodiscard]] const sf::Texture &getTexture(const std::string &path);

		static sf::Vector2f getMapOffset(const sf::Vector2u &windowSize, const sf::Vector2u &bgSize);

	private:
		sf::RenderWindow &window;
		TextureCache textureCache;

		sf::Sprite spriteCache;
		sf::Font font;
		sf::Text textCache;
		bool fontLoaded = false;

		void loadFont();

		// Новый метод для рисования красивых скругленных панелей
		void drawRoundedBox(float x, float y, float w, float h, sf::Color fillColor, sf::Color outlineColor,
							const std::string &title = "");
	};
} // namespace TDEngine::Inner
