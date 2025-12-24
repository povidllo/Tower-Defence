#include "RendererGame.h"
#include <cmath>
#include <filesystem>
#include "../inner/game_objects/GameStatus.h"
#include "../inner/game_objects/MapObject.h"

namespace TDEngine::Inner {

	RendererGame::RendererGame(sf::RenderWindow &window) : window(window) {
		std::cout << "Renderer initialized\n";
		loadFont();
	}

	const sf::Texture &RendererGame::getTexture(const std::string &path) { return textureCache.getTexture(path); }

	void RendererGame::loadFont() {
		std::vector<std::string> fontPaths = {"../../../example_project/Manrope-VariableFont_wght.ttf"};

		for (const auto &path: fontPaths) {
			if (std::filesystem::exists(path) && font.loadFromFile(path)) {
				fontLoaded = true;
				std::cout << "[INFO] Font loaded: " << path << std::endl;
				return;
			}
		}
		std::cerr << "[WARNING] No font found! Text will not be visible.\n";
	}

	sf::Vector2f RendererGame::getMapOffset(const sf::Vector2u &windowSize, const sf::Vector2u &bgSize) {
		float startX = UI_PADDING;
		float startY = UI_TOP_BAR_HEIGHT + UI_PADDING;
		float areaW = UI_SIDEBAR_X - 2 * UI_PADDING;
		float areaH = static_cast<float>(windowSize.y) - UI_TOP_BAR_HEIGHT - 2 * UI_PADDING;

		float mapW = static_cast<float>(bgSize.x);
		float mapH = static_cast<float>(bgSize.y);

		float offsetX = startX + (areaW - mapW) / 2.0f;
		float offsetY = startY + (areaH - mapH) / 2.0f;

		return {std::floor(offsetX), std::floor(offsetY)};
	}

	void RendererGame::drawRoundedBox(float x, float y, float w, float h, sf::Color fillColor, sf::Color outlineColor,
									  const std::string &title) {
		sf::ConvexShape rRect;
		rRect.setPointCount(40);

		float r = CORNER_RADIUS;

		if (w < 2 * r)
			r = w / 2;
		if (h < 2 * r)
			r = h / 2;

		int idx = 0;

		for (int i = 0; i < 10; ++i) {
			float ang = 0 + i * (90.0f / 9.0f);
			float rad = ang * 3.14159f / 180.0f;
			rRect.setPoint(idx++, {x + w - r + r * std::cos(rad), y + r + r * std::sin(rad - 3.14159f / 2)});
		}

		for (int i = 0; i < 10; ++i) {
			float ang = 90 + i * (90.0f / 9.0f);
			float rad = ang * 3.14159f / 180.0f;
			rRect.setPoint(idx++, {x + w - r + r * std::cos(rad), y + h - r + r * std::sin(rad - 3.14159f / 2)});
		}

		for (int i = 0; i < 10; ++i) {
			float ang = 180 + i * (90.0f / 9.0f);
			float rad = ang * 3.14159f / 180.0f;
			rRect.setPoint(idx++, {x + r + r * std::cos(rad), y + h - r + r * std::sin(rad - 3.14159f / 2)});
		}

		for (int i = 0; i < 10; ++i) {
			float ang = 270 + i * (90.0f / 9.0f);
			float rad = ang * 3.14159f / 180.0f;
			rRect.setPoint(idx++, {x + r + r * std::cos(rad), y + r + r * std::sin(rad - 3.14159f / 2)});
		}

		rRect.setFillColor(fillColor);
		rRect.setOutlineColor(outlineColor);
		rRect.setOutlineThickness(2.0f);
		window.draw(rRect);

		if (!title.empty() && fontLoaded) {
			textCache.setFont(font);
			textCache.setString(title);
			textCache.setCharacterSize(18);
			textCache.setFillColor(sf::Color(220, 220, 220));
			sf::FloatRect bounds = textCache.getLocalBounds();
			textCache.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top);
			textCache.setPosition(x + w / 2.0f, y + 8.0f);
			window.draw(textCache);

			sf::RectangleShape line(sf::Vector2f(w - 30, 1));
			line.setPosition(x + 15, y + 35);
			line.setFillColor(sf::Color(100, 100, 100));
			window.draw(line);
		}
	}

	void RendererGame::renderScene(const std::shared_ptr<GameStatus> &gameStat, const sf::Sprite &background) {
		window.setView(window.getDefaultView());

		float zoneX = UI_PADDING;
		float zoneY = UI_TOP_BAR_HEIGHT + UI_PADDING;
		float zoneW = UI_SIDEBAR_X - 2 * UI_PADDING;
		float zoneH = static_cast<float>(window.getSize().y) - UI_TOP_BAR_HEIGHT - 2 * UI_PADDING;

		drawRoundedBox(zoneX, zoneY, zoneW, zoneH, sf::Color(25, 25, 30), sf::Color(60, 60, 65));

		sf::Vector2u bgSize = background.getTexture() ? background.getTexture()->getSize() : sf::Vector2u(0, 0);
		sf::Vector2f offset = getMapOffset(window.getSize(), bgSize);

		sf::Sprite bgCopy = background;
		bgCopy.setPosition(offset.x, offset.y);
		window.draw(bgCopy);

		if (!gameStat)
			return;

		for (const auto &obj: gameStat->mapObjects) {
			const auto &texture = textureCache.getTexture(obj->texturePath);
			spriteCache.setTexture(texture, true);

			float drawX = static_cast<float>(obj->positionCoordinates.first) * TILE_SIZE + offset.x;
			float drawY = static_cast<float>(obj->positionCoordinates.second) * TILE_SIZE + offset.y;

			spriteCache.setPosition(drawX, drawY);
			window.draw(spriteCache);
		}
	}

	void RendererGame::renderUI(const std::shared_ptr<GameStatus> &gameStat,
								const std::vector<UpgradeOption> &upgradeOptions) {
		window.setView(window.getDefaultView());
		if (!gameStat)
			return;

		float winW = static_cast<float>(window.getSize().x);
		float winH = static_cast<float>(window.getSize().y);

		// --- 1. Top Status Bar ---
		float statX = UI_PADDING;
		float statY = UI_PADDING;
		float statW = UI_SIDEBAR_X - 2 * UI_PADDING;
		float statH = UI_TOP_BAR_HEIGHT - 2 * UI_PADDING;

		drawRoundedBox(statX, statY, statW, statH, sf::Color(40, 42, 48), sf::Color(100, 100, 120));

		if (fontLoaded) {
			textCache.setFont(font);
			textCache.setCharacterSize(22);
			textCache.setStyle(sf::Text::Bold);

			textCache.setString("HP: " + std::to_string(gameStat->currentHp));

			sf::FloatRect hpBounds = textCache.getLocalBounds();
			textCache.setOrigin(0, hpBounds.top + hpBounds.height / 2.0f);
			textCache.setPosition(statX + 20.f, statY + statH / 2.0f);

			textCache.setFillColor(sf::Color(235, 80, 80));
			window.draw(textCache);

			float nextX = statX + 40.f + hpBounds.width + 20.0f;
			textCache.setString("Gold: " + std::to_string(gameStat->currentGold));

			sf::FloatRect goldBounds = textCache.getLocalBounds();
			textCache.setOrigin(0, goldBounds.top + goldBounds.height / 2.0f);
			textCache.setPosition(nextX, statY + statH / 2.0f);

			textCache.setFillColor(sf::Color(255, 215, 0));
			window.draw(textCache);
		}

		float sideX = UI_SIDEBAR_X + UI_PADDING;
		float sideY = UI_PADDING;
		float sideW = winW - UI_SIDEBAR_X - 2 * UI_PADDING;
		float sideH = winH - 2 * UI_PADDING;

		drawRoundedBox(sideX, sideY, sideW, sideH, sf::Color(35, 37, 43), sf::Color(80, 80, 90), "ACTIONS");

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldMouse = window.mapPixelToCoords(mousePos);

		for (const auto &opt: upgradeOptions) {
			bool hover = opt.bounds.contains(worldMouse);

			sf::Color bodyColor = hover ? sf::Color(70, 80, 100) : sf::Color(55, 55, 60);
			sf::Color borderColor = hover ? sf::Color::White : sf::Color(120, 120, 130);

			drawRoundedBox(opt.bounds.left, opt.bounds.top, opt.bounds.width, opt.bounds.height, bodyColor, borderColor,
						   "");

			float paddingInside = 5.0f;
			float iconSize = opt.bounds.height - (paddingInside * 2);

			if (opt.texture) {
				sf::Sprite icon(*opt.texture);
				float scaleX = iconSize / opt.texture->getSize().x;
				float scaleY = iconSize / opt.texture->getSize().y;
				icon.setScale(scaleX, scaleY);
				icon.setPosition(opt.bounds.left + paddingInside, opt.bounds.top + paddingInside);
				window.draw(icon);
			}

			if (fontLoaded) {
				textCache.setString(opt.name);
				textCache.setCharacterSize(14);
				textCache.setStyle(sf::Text::Regular);
				textCache.setFillColor(sf::Color(220, 220, 220));

				sf::FloatRect textBounds = textCache.getLocalBounds();

				textCache.setOrigin(0, textBounds.top + textBounds.height / 2.0f);

				float textX = opt.bounds.left + paddingInside + iconSize + 10.0f;
				float textY = opt.bounds.top + opt.bounds.height / 2.0f;

				textCache.setPosition(textX, textY);
				window.draw(textCache);
			}
		}
	}
	void RendererGame::renderMenu(const std::vector<MenuButton> &buttons) {
		window.setView(window.getDefaultView());
		window.clear(sf::Color(30, 32, 36));

		if (fontLoaded) {
			textCache.setFont(font);
			textCache.setString("SELECT MAP");
			textCache.setCharacterSize(40);
			textCache.setFillColor(sf::Color::White);
			textCache.setStyle(sf::Text::Bold);
			sf::FloatRect tr = textCache.getLocalBounds();
			textCache.setOrigin(tr.left + tr.width / 2.0f, tr.top + tr.height / 2.0f);
			textCache.setPosition(window.getSize().x / 2.0f, 100.0f);
			window.draw(textCache);
		}

		for (const auto &btn: buttons) {
			sf::Color fill = btn.isHovered ? sf::Color(70, 90, 120) : sf::Color(50, 55, 60);
			sf::Color outline = btn.isHovered ? sf::Color::White : sf::Color(100, 100, 100);

			drawRoundedBox(btn.bounds.left, btn.bounds.top, btn.bounds.width, btn.bounds.height, fill, outline, "");

			if (fontLoaded) {
				textCache.setFont(font);
				textCache.setString(btn.text);
				textCache.setCharacterSize(24);
				textCache.setFillColor(sf::Color::White);
				textCache.setStyle(sf::Text::Regular);

				sf::FloatRect bTextRect = textCache.getLocalBounds();
				textCache.setOrigin(bTextRect.left + bTextRect.width / 2.0f, bTextRect.top + bTextRect.height / 2.0f);
				textCache.setPosition(btn.bounds.left + btn.bounds.width / 2.0f,
									  btn.bounds.top + btn.bounds.height / 2.0f);

				window.draw(textCache);
			}
		}
	}

	void RendererGame::renderGameOver(bool victory) {
		window.setView(window.getDefaultView());
		sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
		overlay.setFillColor(sf::Color(0, 0, 0, 200));
		window.draw(overlay);

		if (!fontLoaded)
			return;
		textCache.setFont(font);

		std::string titleText = victory ? "VICTORY!" : "DEFEAT";
		sf::Color titleColor = victory ? sf::Color::Green : sf::Color::Red;

		textCache.setString(titleText);
		textCache.setCharacterSize(60);
		textCache.setFillColor(titleColor);
		textCache.setStyle(sf::Text::Bold);
		sf::FloatRect bounds = textCache.getLocalBounds();
		textCache.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
		textCache.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
		window.draw(textCache);
	}

} // namespace TDEngine::Inner
