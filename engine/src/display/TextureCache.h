#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

namespace TDEngine::Inner {

	class TextureCache {
	private:
		std::unordered_map<std::string, sf::Texture> cache;
		sf::Texture fallbackTexture;

		void createFallback() {
			if (!fallbackTexture.create(32, 32))
				return;
			sf::Uint8 pixels[32 * 32 * 4];
			for (int i = 0; i < 32 * 32 * 4; i += 4) {
				pixels[i] = 255;
				pixels[i + 1] = 0;
				pixels[i + 2] = 255;
				pixels[i + 3] = 255;
			}
			fallbackTexture.update(pixels);
		}

	public:
		TextureCache() { createFallback(); }

		const sf::Texture &getTexture(const std::string &path) {

			if (path.empty())
				return fallbackTexture;

			if (auto it = cache.find(path); it != cache.end()) {
				return it->second;
			}

			sf::Texture tex;
			if (!tex.loadFromFile(path)) {
				std::cerr << "[WARNING] Failed to load texture: " << path << ". Using fallback.\n";
				cache[path] = fallbackTexture;
				return cache[path];
			}

			cache[path] = std::move(tex);
			return cache[path];
		}
	};

} // namespace TDEngine::Inner
