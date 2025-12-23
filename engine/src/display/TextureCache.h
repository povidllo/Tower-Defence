#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

namespace TDEngine::Inner {

	class TextureCache {
	private:
		std::unordered_map<std::string, sf::Texture> cache;
		sf::Texture fallbackTexture; // Текстура-заглушка (например, фиолетовый квадрат)

		void createFallback() {
			if (!fallbackTexture.create(32, 32))
				return;
			sf::Uint8 pixels[32 * 32 * 4];
			for (int i = 0; i < 32 * 32 * 4; i += 4) {
				pixels[i] = 255; // R
				pixels[i + 1] = 0; // G
				pixels[i + 2] = 255; // B (Magenta)
				pixels[i + 3] = 255; // A
			}
			fallbackTexture.update(pixels);
		}

	public:
		TextureCache() { createFallback(); }

		// Возвращаем const reference, чтобы никто случайно не изменил текстуру в кэше
		const sf::Texture &getTexture(const std::string &path) {
			// Если путь пустой, возвращаем заглушку
			if (path.empty())
				return fallbackTexture;

			// Если текстура уже есть в кэше
			if (auto it = cache.find(path); it != cache.end()) {
				return it->second;
			}

			// Пытаемся загрузить
			sf::Texture tex;
			if (!tex.loadFromFile(path)) {
				std::cerr << "[WARNING] Failed to load texture: " << path << ". Using fallback.\n";
				// Запоминаем, что этот путь битый, чтобы не пытаться грузить каждый кадр
				// Можно сохранить fallbackTexture под этим ключом
				cache[path] = fallbackTexture;
				return cache[path];
			}

			cache[path] = std::move(tex);
			return cache[path];
		}
	};

} // namespace TDEngine::Inner
